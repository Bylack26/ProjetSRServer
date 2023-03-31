# ProjetSRServer

Ne pas fermer le terminal avec CTRL-D quand le serveur est en background;

Protocol : 
- Envoie côté client du nom du fichier a rechercher dans les dossiers du serveur
- Reception côté serveur avec Rio_readlineb
- Envoie côté serveur de la taille du fichier
- Reception côté client et création d'un buffer au bonne dimension
- Envoie côté serveur des paquets de données lu depuis le fichier et reception et traitement de chaque paquets les uns après les autres

Paquets :
- Tableau de taille maximale 1000 de char
- Taille effective du tableau (1000 pour des paquets "classique" ou moins pour des petits paquets)

