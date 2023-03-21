#include "csapp.h"

#define MAX_NAME_LEN 256
int nbProcess = 0;
void echo(int connfd);

void handlerChild(int sig){
    pid_t pid;
    printf("Avant le kill : %d\n", nbProcess);
    nbProcess--;
    printf("Aprés le kill : %d\n", nbProcess);
    while((pid = waitpid(-1, NULL, WNOHANG | WUNTRACED)) > 0){

    }
}



int main(int argc, char **argv)
{
    int listenfd, connfd, port;
    socklen_t clientlen;
    struct sockaddr_in clientaddr;
    char client_ip_string[INET_ADDRSTRLEN];
    char client_hostname[MAX_NAME_LEN];
    port = 2121;
    pid_t pid;
    
    clientlen = (socklen_t)sizeof(clientaddr);

    listenfd = Open_listenfd(port);
    Signal(SIGCHLD, handlerChild);
    while (1) {
        
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        nbProcess ++;
        pid = Fork();

        if(pid == 0){
            /* determine the name of the client */
            Getnameinfo((SA *) &clientaddr, clientlen,
                        client_hostname, MAX_NAME_LEN, 0, 0, 0);
            
            /* determine the textual representation of the client's IP address */
            Inet_ntop(AF_INET, &clientaddr.sin_addr, client_ip_string,
                        INET_ADDRSTRLEN);
            
            printf("server connected to %s (%s)\n", client_hostname,
                    client_ip_string); 
            echo(connfd);
            Close(connfd);
            exit(0);
        }else{
            Close(connfd);

        }
        
    }
    exit(0);
}

