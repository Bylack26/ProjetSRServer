# ProjetSRServer

Protocol : 
- Envoie côté client du nom du fichier a rechercher dans les dossiers du serveur
- Reception côté serveur avec Rio_readlineb
- Envoie côté serveur de la taille du fichier
- Reception côté client et création d'un buffer au bonne dimension
- Envoie côté serveur des paquets de données lu depuis le fichier et reception et traitement de chaque paquets les uns après les autres

Paquets :
- Tableau de taille maximale 1000 de char
- Taille effective du tableau (1000 pour des paquets "classique" ou moins pour des petits paquets)

Utilisation:

- Compilation : "make clean" nettoie le dossier de téléchargement du client et supprime les 2 executable "client" et "server"
- Lancer le serveur : "./server"
- Lancer le client : "./client" prend le DNS du serveur en paramètre

Commande:
- "get" : attends la saisie sur l'entrée standard du chemin vers un fichier connu du serveur.
Récupère et copie le fichier par paquets.
- "bye" : coupe la connexion avec le serveur et arrête le client.