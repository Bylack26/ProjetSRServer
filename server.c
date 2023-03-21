#include "csapp.h"

#define MAX_NAME_LEN 256
#define NBPROCMAX 3


pid_t tabpid[NBPROCMAX];

void echo(int connfd);

void handlerChild(int sig){
    pid_t pid;
    while((pid = waitpid(-1, NULL, WNOHANG | WUNTRACED)) > 0){

    }
}
void handlerINT(int sig){
    int i = 0;
    while(i < NBPROCMAX){
        Kill(tabpid[i],SIGINT);
        i++;
    }
    Kill(getpid(),SIGINT);

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
    Signal(SIGCHLD, handlerChild);
    listenfd = Open_listenfd(port); // socket de connection
    int nbps=0;
    while(nbps < NBPROCMAX ){
        pid = Fork();
        tabpid[nbps]= pid;
        nbps++;
        if(pid == 0){
            break;
        }
    }
    if(pid != 0){
        Signal(SIGINT, handlerINT);
        Signal(SIGCHLD, handlerChild);
    }

    while (1) {

        if(pid == 0){
            connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen); // socket de communication
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
        }
    }
    exit(0);
    
}
