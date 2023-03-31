#define LOG ".log"
#define DIR "./DirClient/"
struct Log{
    int lastBloc;
    char tailleNom;
    char * name;
};


int recupereFichier(int clientfd, rio_t rio);
/*
 * Ecris le nom du fichier en cours de téléchargement dans un fichier log
 * Ecris aussi l'indice du dernier bloc reçu et écrit.
 * Ecrit aussi le nombre de blocs total dans le téléchargement.
 */
void ecritureLog(int id, int nbBloc, char * name);
struct Log lectureLog(int fdLog);
struct Log * crashed();
void recuperePartiel(int clientfd, rio_t rio, char * nom, int id);