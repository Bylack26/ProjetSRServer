/*
 * echo - read and echo text lines until client closes connection
 */
#include "csapp.h"
#include "serverFunction.h"
void echo(int connfd){
    size_t n;
    char buf[MAXLINE];
    rio_t rio;

    Rio_readinitb(&rio, connfd);
    if((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) {
        printf("server received %u bytes\n", (unsigned int)n);
        Rio_writen(connfd, buf, n);
    }
}

void taille(int connfd){
    rio_t rio;
    char * buf = (char *)calloc(MAXLINE,sizeof(char));

    Rio_readinitb(&rio, connfd);
    size_t n = Rio_readlineb(&rio, buf, MAXLINE);
    char fichier[n];

    for(int i =0; i < n-1; i++){
        fichier[i] = buf[i];
    }
    fichier[n-1] = '\0';
    fprintf(stderr, "%s\n", fichier);



    int f = open(fichier, (DEF_MODE) & ~(DEF_UMASK), "r");
    if(f < 0){
        fprintf(stderr, "une erreur est survenue avec le descripteur %d\n", f);
        int erreur = -1;
        Rio_writen(connfd, &erreur, sizeof(int));// On envoie le code d'erreur
    }else{
        //Récupération de la taille
        struct stat * s = malloc(sizeof(struct stat));
        Fstat(f, s);
        fprintf(stderr, "off_t %ld\n", s->st_size);
        int dim = s->st_size;
        Rio_writen(connfd, &dim, sizeof(int));
        int nbBloc = dim/ PAQUET_SIZE;
        if(dim%PAQUET_SIZE){
            nbBloc += 1;
        }
        int i = 0;
        struct paquet *p;
        p = calloc(1,sizeof(struct paquet));

        Rio_writen(connfd, &nbBloc, sizeof(int));
        while(i < nbBloc){
            p->id = i;
            envoiePaquet(connfd, p, f);
            i++;   
        }
        

    }

}


void envoiePaquet(int connfd, struct paquet * p, int f){
        int n = (int)read(f, p->data, sizeof(char)* PAQUET_SIZE);
        p->size = n;
        //fprintf(stderr, "Cote serveur %ld\n", p->size);
        Rio_writen(connfd, p, sizeof(struct paquet));   
}


char getCommand(int connfd){
    char command;
    rio_t rio;
    Rio_readinitb(&rio, connfd);
    Rio_readn(rio.rio_fd, &command , sizeof(char));
    return command;
}