/*
 * echo - read and echo text lines until client closes connection
 */
#include "csapp.h"

void echo(int connfd)
{
    size_t n;
    char buf[MAXLINE];
    rio_t rio;

    Rio_readinitb(&rio, connfd);
    while ((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) {
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

    fprintf(stderr, "%sa\n", fichier);
    int f = open(fichier, (DEF_MODE) & ~(DEF_UMASK), "r");
    struct stat * s = malloc(sizeof(struct stat));
    Fstat(f, s);
    printf("off_t %ld", s->st_size);
    int dim = s->st_size;
    Rio_writen(connfd, &dim, 4);
    

}

