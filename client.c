#include "csapp.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "fixed_serv.h"

char * name(char * nomComplet, int size){
    int i = size -1;
    while(i >= 0 && nomComplet[i] != '/'){
        i--;
    }
    i++;
    char * nomFichier = calloc(size -i+1, sizeof(char));
    int j =0;
    while(i < size){
        nomFichier[j] = nomComplet[i];
        j++;
        i++;
    }
    nomFichier[j] = '\0';
    return nomFichier;
}




int main(int argc, char **argv)
{
    int clientfd, port;
    char *host, buf[MAXLINE];
    char dataSize[PAQUET_SIZE];
    char * dossier = "./DirClient/";
    int taille;
    rio_t rio;
    if (argc != 2) {
        fprintf(stderr, "usage: %s <host> \n", argv[0]);
        exit(0);
    }

    host = argv[1];
    port = 2121;

    clientfd = Open_clientfd(host, port);
    
    printf("client connected to server OS\n"); 
    
    Rio_readinitb(&rio, clientfd);

    if(Fgets(buf, MAXLINE, stdin) != NULL) {
        // On envoie le nom du fichier
        clock_t begin = clock();
        Rio_writen(clientfd, buf, strlen(buf)); 
        // On récupère la taille du fichier
        if(Rio_readn(rio.rio_fd, &taille, sizeof(int)) > 0){
            if(taille < 0){
                char * message_erreur = "Le fichier n'existe pas";
                fprintf(stderr, "%s\n", message_erreur);
                exit(1);
            }else{
                char * dossierSortie = calloc(strlen(dossier)+strlen(buf), sizeof(char));
                strcat(dossierSortie, dossier);
                int sortie = open(strcat(dossierSortie, name(buf, strlen(buf))),O_CREAT | O_RDWR ,S_IRWXU );
                if(sortie < 0){
                    fprintf(stderr,"Impossible d'ecrire en sortie\n");
                    exit(1);
                }
                
                ssize_t paquet = 0;
                struct paquet * p = calloc(1, sizeof(struct paquet));
                while(Rio_readn(rio.rio_fd, p, sizeof(struct paquet)) > 0){
                    fprintf(stderr, "%d\n", p->size);
                    write(sortie, p->data, p->size);
                }
                clock_t end = clock();
                double millis = ((double)end-(double)begin)*1000/CLOCKS_PER_SEC;
                fprintf(stdout, "%d octet(s) transféré en %f milli-secondes (%f Octets/ms)\n",taille+4, millis, ((taille+4) / (millis)));

            }
        }
    }
    Close(clientfd);
    exit(0);
}

