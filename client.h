#define LOG ".log"
#define DIR "./DirClient/"
struct Log{
    int lastBloc;
    int maxBlock;
    char * name;
};


int recupereFichier(int clientfd, rio_t rio);
/*
 * Ecris le nom du fichier en cours de téléchargement dans un fichier log
 * Ecris aussi l'indice du dernier bloc reçu et écrit.
 * Ecrit aussi le nombre de blocs total dans le téléchargement.
 */
void ecritureLog(int id, int nbBloc,int fdLog, char * name);
struct Log lectureLog(int fdLog);