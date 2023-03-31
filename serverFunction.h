#define PAQUET_SIZE 1000
#define GET "get\n"
#define BYE "bye\n"
#define GET_FUNC 1
#define BYE_FUNC 0
#define UNRECOGNIZE 0xFF
struct paquet{
    ssize_t size;
    char data[PAQUET_SIZE];
    int id;
};


void envoiePaquet(int connfd, struct paquet * p, int f);
void taille(int connfd);
void echo(int connfd);
char getCommand(int connfd);