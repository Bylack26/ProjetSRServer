#include "csapp.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "serverFunction.h"
#include "client.h"
#include <unistd.h>

//Handler lors d'un crash du serveur
void crashServeur(int sig){
    fprintf(stderr, "Le serveur a rencontré des problèmes\n");
    exit(2);
}


char * fichierServ(char * nom){
    char * c = calloc(strlen(nom), sizeof(char));
    int i =0;
    while(i < strlen(nom)){
        c[i] = nom[i];
        i++;
    }
    return c;
}

char * name(char * nomComplet, int size){
    int i = size -1;
    while(i >= 0 && nomComplet[i] != '/'){
        i--;
    }
    i++;
    int taille = size -i+1;
    char * nomFichier = calloc(taille, sizeof(char));
    int j =0;
    while(i < size && nomComplet[i] != '\n'){
        nomFichier[j] = nomComplet[i];
        j++;
        i++;
    }
    for(;j < taille; j++){
        nomFichier[j] = '\0';
    }
    return nomFichier;
}

int main(int argc, char **argv){
    int clientfd, port;
    char *host, buf[MAXLINE];    
    char command = START;
    struct Log * log;
    rio_t rio;
    if (argc != 2) {
        fprintf(stderr, "usage: %s <host> \n", argv[0]);
        exit(0);
    }

    //Initialisation de la communication
    host = argv[1];
    port = 2121;

    clientfd = Open_clientfd(host, port);
    
    printf("client connected to server OS\n"); 
    Signal(SIGPIPE, crashServeur);

    Rio_readinitb(&rio, clientfd); 
    if((log = crashed())!= NULL){
        //Reprise de téléchargement
        
        command = REPRISE_FUNC;
        Rio_writen(clientfd, &command, sizeof(char));
        Rio_writen(clientfd, &log->tailleNom, sizeof(char));
        Rio_writen(clientfd, log->name, log->tailleNom*sizeof(char));
        Rio_writen(clientfd, &log->lastBloc, sizeof(int));
        recuperePartiel(clientfd, rio, log->name, log->lastBloc);
        command = START;
        goto recuperationFinit;
    }else{
recuperationFinit:
            //Tant que command est different de 0 c'est à dire "bye"
            while(command ){
            fprintf(stdout,"Entrez une commande\n");
            //On récupère la commande sur l'entrée standard
            if(Fgets(buf, MAXLINE, stdin) != NULL){
                //Traitement selon commande
                command = strcmp(buf, BYE);
                if(command){
                    //Si la commande est "get"
                    if(!strcmp(buf, GET)){
                        command = GET_FUNC;
                        Rio_writen(clientfd, &command, sizeof(char)); 
                        recupereFichier(clientfd, rio);
                    }else{
                        fprintf(stderr, "Commande non reconnue\n");
                        command = UNRECOGNIZE;
                    }

                }else{
                    Rio_writen(clientfd, &command, sizeof(char));
                }

            }else{
                command = UNRECOGNIZE;
                fprintf(stderr, "Aucune commande entrée\n");
            }
            //fprintf(stderr, "sortie client\n");
        }
    }

    Close(clientfd);
    exit(0);
}

//On récupère le fichier depuis le serveur
int recupereFichier(int clientfd, rio_t rio){
    int taille = 1;
    int nbOctetReceived = 0;
    int nbBloc;
    char * buf = calloc(MAXLINE, sizeof(char));
    if(Fgets(buf, MAXLINE, stdin) != NULL) {
        // On envoie le nom du fichier
        clock_t begin = clock();
        Rio_writen(clientfd, buf, strlen(buf)); 
        // On récupère la taille du fichier
        Rio_readn(rio.rio_fd, &taille, sizeof(int));
        if(taille < 0){
            char * message_erreur = "Le fichier n'existe pas";
            fprintf(stderr, "%s\n", message_erreur);
        }else{

            char * dossierSortie = calloc(strlen(DIR)+strlen(buf), sizeof(char));
            strcat(dossierSortie, DIR);
            int sortie = open(strcat(dossierSortie, name(buf, strlen(buf))),O_CREAT | O_RDWR ,S_IRWXO| S_IRWXU | S_IRWXG);
            
            if(sortie < 0){
                fprintf(stderr,"Impossible d'ecrire en sortie\n");
                Close(clientfd);
                exit(1);
            }
            //On crée un fichier de log dans un répertoire caché

            struct paquet * p = calloc(1, sizeof(struct paquet));
            Rio_readn(rio.rio_fd, &nbBloc, sizeof(int));
            int i = 0;
            while(i < nbBloc && Rio_readn(rio.rio_fd, p, sizeof(struct paquet)) > 0){
                
                nbOctetReceived += p->size;
                write(sortie, p->data, p->size);
                
                ecritureLog(p->id, nbBloc, fichierServ(buf));
                //sleep(2);
                i++;
            }
            clock_t end = clock();
            if(i == nbBloc){
                remove("./DirClient/.log");
            }
            double millis = ((double)end-(double)begin)*1000/CLOCKS_PER_SEC;
            fprintf(stdout, "%d octet(s) transféré en %f milli-secondes (%f Octets/ms)\n",nbOctetReceived, millis, ((nbOctetReceived) / (millis)));
        }
    }
    return nbOctetReceived;
}

//Ecriture du fichier de log
void ecritureLog(int id, int nbBloc, char * name){
    char separation = '|';
    char * logDos = calloc(strlen(DIR)+strlen(LOG), sizeof(char));
    strcat(logDos, DIR);
    strcat(logDos, LOG);
    //Suppresion du fichier si il existe 
    remove(logDos);
    //Récriture des nouvelles données dans un fichier ".log"
    int fdLog = open(logDos, O_CREAT | O_RDWR ,S_IRWXO| S_IRWXU | S_IRWXG );
    if(fdLog < 0){
        fprintf(stderr,"Une erreur de log est survenue\n");
        exit(1);
    }
    char tailleNom = (char)strlen(name);
    write(fdLog, &tailleNom, sizeof(char));
    write(fdLog, name, sizeof(char)*strlen(name));
    write(fdLog, &separation , sizeof(char));
    write(fdLog, &id, sizeof(int));
}

//Indique si le téléchargement à échoué précédemment
struct Log * crashed(){
    char * logDos = calloc(strlen(DIR)+strlen(LOG), sizeof(char));
    strcat(logDos, DIR);
    strcat(logDos, LOG);
    int fdLog = open(logDos, O_RDONLY);

    //Lecture du fichier log
    if(fdLog != -1){
        struct Log * log = calloc(1, sizeof(struct Log));
        char c;
        //Récuperation de la taille du nom (premiers octets)
        int n = (int)read(fdLog, &c, sizeof(char));
        log->tailleNom = c;
        int i = 0;
        //Recuperation du chemin complet du fichier(dans un char * de la taille précédemment récupéré)
        log->name = calloc(c, sizeof(char));
        n = (int)read(fdLog, &c, sizeof(char));
        while(n == 1 &&  c != '|'){
            log->name[i] = c;
            n = (int)read(fdLog, &c, sizeof(char));
            i++;
        }
        if(c != '|'){
            fprintf(stderr,"Fichier log corrompu\n");
            return NULL;
        }
        int id;
        //Lecture de l'id du dernier paquet lu
        n = (int)read(fdLog, &id, sizeof(int));
        log->lastBloc = id+1;
        return log;
        
    }else{
        return NULL;
    }
    
}


//Récupération d'une partie d'un document dont le téléchargement a été interrompu.
void recuperePartiel(int clientfd, rio_t rio, char * nom, int id){
    clock_t begin = clock();
    int nbOctetReceived = 0;
    int nbBloc;

    //Ouverture du fichier en cours de téléchargement
    char * dossierSortie = calloc(strlen(DIR)+strlen(nom), sizeof(char));
    strcat(dossierSortie, DIR);
    strcat(dossierSortie, name(nom, strlen(nom)));
    int sortie = open(dossierSortie,O_APPEND | O_RDWR);
    
    //Si le fichier n'existe plus suppression du log et remise du client enattente de commande
    Rio_writen(clientfd, &sortie, sizeof(int)); 
    if(sortie < 0){
        fprintf(stderr,"Impossible de reprendre le téléchargment\n Erreur :  %s\n", strerror(errno));
        remove("./DirClient/.log");
        //On ignore tout le code si le fichier n'existe pas
        goto aucunFichier;
    }
    
    
    struct paquet * p = calloc(1, sizeof(struct paquet));
    //Lecture du nombre de paquet a recevoir
    Rio_readn(rio.rio_fd, &nbBloc, sizeof(int));
    int i = id;
    //récupération des paquets manquants
    while(i < nbBloc && Rio_readn(rio.rio_fd, p, sizeof(struct paquet)) > 0){
        
        nbOctetReceived += p->size;
        write(sortie, p->data, p->size);
        ecritureLog(p->id, nbBloc, fichierServ(nom));
        i++;
    }
    clock_t end = clock();
    free(p);
    //Effacement du fichier log à la fin du téléchargement
    remove("./DirClient/.log");
    double millis = ((double)end-(double)begin)*1000/CLOCKS_PER_SEC;
    fprintf(stdout, "%d octet(s) transféré en %f milli-secondes (%f Octets/ms)\n",nbOctetReceived, millis, ((nbOctetReceived) / (millis)));
aucunFichier:
}


