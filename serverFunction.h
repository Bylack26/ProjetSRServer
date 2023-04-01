#define PAQUET_SIZE 1000
#define GET "get\n"
#define BYE "bye\n"
#define BYE_FUNC 0
#define GET_FUNC 1
#define REPRISE_FUNC 2
#define START 0xFE
#define UNRECOGNIZE 0xFF
#define MAX_NAME_LEN 256
#define NBPROCMAX 5

struct paquet{
    ssize_t size;
    char data[PAQUET_SIZE];
    int id;
};

/*
Envoie les paquets du fichier f, au client connecté sur connfd
*/
void envoiePaquet(int connfd, struct paquet * p, int f);

/*
Prends en charge la lecture du nom du fichier et son envoie au client au moyen de envoiePaquet
*/
int taille(int connfd);


/*
Récupération d'une commande depuis le client et renvoie cette commande en sortie
*/
char getCommand(int connfd);

/*
Prends en charge la réception des logs d'un client et relance un téléchargement 
*/
int reprise(int connfd);


/*
Envoie les paquets id+1 jusqu'à la fin, du fichier fd
*/
int envoieReprise(int connfd, rio_t rio, int fd, int id);