#include "csapp.h"

void lecteur(char *fichier, int connfd)
{
    size_t n;
    char buf[MAXLINE];
    rio_t rio;
    int descripteur 
    Rio_readinitb(&rio, connfd);
    printf("server received %u bytes\n", (unsigned int)n);
    Rio_writen(connfd, buf, n);
}
