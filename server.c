#include "csapp.h"
#include <stdlib.h>
#include <stdio.h>
#include "serverFunction.h"


pid_t tabpid[NBPROCMAX];

void crashClient(int sig){
    fprintf(stderr, "Le client s'est déconnecté\n");
}

void handlerChild(int sig){
    pid_t pid;
    while((pid = waitpid(-1, NULL, WNOHANG | WUNTRACED)) > 0){

    }
}

void handlerINT(int sig){
    Kill(0,SIGKILL);
}

int main(int argc, char **argv){
    int listenfd, connfd, port;
    char command =1;
    int crash = 0;
    socklen_t clientlen;
    struct sockaddr_in clientaddr;
    char client_ip_string[INET_ADDRSTRLEN];
    char client_hostname[MAX_NAME_LEN];
    port = 2121;
    pid_t pid;
    clientlen = (socklen_t)sizeof(clientaddr);
    //Signal(SIGPIPE, crashClient);
    listenfd = Open_listenfd(port); // socket de connection
    int nbps=0;
    if(argc < 0){

    }
    while(nbps < NBPROCMAX ){
        pid = Fork();
        tabpid[nbps]= pid;
        nbps++;
        if(pid == 0){
            break;
        }
    }
    if(pid != 0){
        Close(listenfd);
        Signal(SIGINT, handlerINT);
        Signal(SIGCHLD, handlerChild);
    }

    

    while (1) {
        while((connfd = accept(listenfd, (SA *)&clientaddr, &clientlen)) < 0);
        // socket de communication
        /* determine the name of the client */
        Getnameinfo((SA *) &clientaddr, clientlen, client_hostname, MAX_NAME_LEN, 0, 0, 0);
        
        /* determine the textual representation of the client's IP address */
        Inet_ntop(AF_INET, &clientaddr.sin_addr, client_ip_string, INET_ADDRSTRLEN);
        
        printf("server connected to %s (%s)\n", client_hostname, client_ip_string); 
        command = getCommand(connfd);
        while(command){
            fprintf(stderr, "Boucle cote serveur %d\n", command);
            switch (command){
                case GET_FUNC:
                    crash = taille(connfd);
                    break;
                case REPRISE_FUNC:
                    crash = reprise(connfd);
                    break;
                default:
                    break;
            }

            if(!crash){
                fprintf(stderr,"On break\n");
                break;
            }
            fprintf(stderr, "Sortie serveur\n");
            command = getCommand(connfd);
        }
        fprintf(stderr,"Connection coupé\n");
        Close(connfd);  
    }
    exit(0);
    
}