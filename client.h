#define LOG ".log"
#define DIR "./DirClient/"
struct Log{
    int lastBloc;
    char tailleNom;
    char * name;
};
/*
Tronque les '\0' dans un nom de fichier
*/
char * fichierServ(char * nom);

/*
Récupère les paquets envoyés par le serveur et les ecris dans un fichier tout en lettant à jour un fichier de log
*/
int recupereFichier(int clientfd, rio_t rio);

/*
Ecris le nom du fichier en cours de téléchargement dans un fichier log
Ecris aussi l'indice du dernier bloc reçu et écrit.
Ecrit aussi le nombre de blocs total dans le téléchargement.
*/
void ecritureLog(int id, int nbBloc, char * name);

/*
Lecture d'un fichier de log et remplissage d'une structures Log.
Renvoie la structures si le fichier existe et a été correctement écris
Renvoie NULL en cas d'abscence de fichier ou de fichier mal formé
*/
struct Log * crashed();

/*
Récupère les paquets envoyés par le serveur et les ecris à la suite dans un fichier dont le téléchargement s'est brusquement arreté
Met a jour un fichier de log
*/
void recuperePartiel(int clientfd, rio_t rio, char * nom, int id);