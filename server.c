#include "csapp.h"

#define MAX_NAME_LEN 256
void echo(int connfd);

handlerChild



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
    while (1) {
        
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
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

