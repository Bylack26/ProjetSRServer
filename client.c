#include "csapp.h"

int main(int argc, char **argv)
{
    int clientfd, port;
    char *host, buf[MAXLINE];
    char taille[4];
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

        Rio_writen(clientfd, buf, strlen(buf)); // On envoie le nom du fichier
        // On récupère le fichier

        // On récupère la taille du fichier
        if(Rio_readn(rio.rio_fd, taille, sizeof(int)) > 0){
            printf("%d\n",taille[0]);
        }
    }
    Close(clientfd);
    exit(0);
}