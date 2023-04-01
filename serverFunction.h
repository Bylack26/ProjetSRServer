#define PAQUET_SIZE 1000
#define GET "get\n"
#define BYE "bye\n"
#define GET_FUNC 1
#define BYE_FUNC 0
#define REPRISE_FUNC 2
#define UNRECOGNIZE 0xFF
#define MAX_NAME_LEN 256
#define NBPROCMAX 5

struct paquet{
    ssize_t size;
    char data[PAQUET_SIZE];
    int id;
};


void envoiePaquet(int connfd, struct paquet * p, int f);
int taille(int connfd);
void echo(int connfd);
char getCommand(int connfd);
int reprise(int connfd);
int envoieReprise(int connfd, rio_t rio, int fd, int id);