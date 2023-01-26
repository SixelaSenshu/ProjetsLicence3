#include "../headers/main.h"

int main(int argc,char ** argv){
    HashmapConnection * map = NULL;
    Reader * reader = NULL;
    int res;
    char * path;

    if(argc != 2){
        printf("Paramètres invalide (1): ./exec <file>");
        return EXIT_FAILURE;
    }
    printf("\n-------------------------------------------------------------------------------------------- Début du programme ---------------------------------------------------------------------------------------------------\n");
    path = argv[1];
    printf("\n------------------------------------------------------------------------------------------ [1]Parsing du texte ----------------------------------------------------------------------------------------------------\n");
    reader = createReader();
    readFile(path,"r",reader);

    printf("[1.1]---------------------------------------------------- Affichage des personnes \n");
    toStringArrayListContact(reader->contacts);
    printf("[1.2]---------------------------------------------------- Affichage des villes \n\n");
    toStringCities(reader->cities);

    printf("\n[1]---------------------------------------------------- Fermeture du fichier \n");
    printf("\nLa moyenne des distances des villes est de %f\n", getDistanceAverage(reader->contacts,reader->cities));
    printf("\n------------------------------------------------------------------------------------------- [2]Graphe de connections ----------------------------------------------------------------------------------------------\n");
    
    map = createHashmapConnection(reader->contacts);
    updateHashmapConnection(map);

    printf("\n[2.1]---------------------------------------------------- Affichage de la matrice de connections \n\n");
    toStringHashmapConnection(map);
    
    res = getBetweenDistanceContacts(reader->contacts->users[0],reader->contacts->users[1],map);
    printf("\nExemple de distance entre 2 personnes:\n");
    printf("Noms :%s - :%s : %d \n",reader->contacts->users[0]->name,reader->contacts->users[1]->name,res);
    res = getBetweenDistanceContacts(reader->contacts->users[1],reader->contacts->users[0],map);
    printf("Noms :%s - :%s : %d \n",reader->contacts->users[1]->name,reader->contacts->users[0]->name,res);
    printf("\n[2.2] La moyenne des distances de contacts entre les personnes est de %f\n", getAverageDistanceContacts(map));

    printf("\n----------------------------------------------------------------------------------- [3]Symétisation du graphe de connections ---------------------------------------------------------------------------------------\n");

    printf("\n[3.1]---------------------------------------------------- Symétrisation des followers \n");
    symmetrizerHashmapConnection(reader->contacts);
    printf("\n[3.2]---------------------------------------------------- Affichage des personnes \n");
    toStringArrayListContact(reader->contacts);

    printf("\n--------------------------------------------------------------------------- [4]Calcul des components connexes du graphe de connections -----------------------------------------------------------------------------\n");

    
    printf("\n[4.1]---------------------------------------------------- Création des composantes connexes\n");
    createComponents(reader->contacts);
    printf("\n[4.2]---------------------------------------------------- Affichage des composantes connexes\n");
    toStringComponents(reader->contacts);

    printf("\n[4.3]---------------------------------------------------- Création des composantes connexes fortes\n");
    createComponentsStrong(reader->contacts);
    printf("\n[4.4]---------------------------------------------------- Affichage des composantes connexes fortes\n");
    toStringComponentsStrong(reader->contacts);

    printf("\n------------------------------------------------------------------------------------ [5]Calcul via l'algorithme de Tarjan  ----------------------------------------------------------------------------------------\n");

    printf("\n[5.1]---------------------------------------------------- Création des composantes connexes fortes via l'algorithme de Tarjan.\n");
    createComponentsStrongTarjan(reader->contacts);
    printf("\n[5.2]---------------------------------------------------- Affichage des composantes connexes fortes via l'algorithme de Tarjan.\n");
    toStringComponentsStrongTarjan(reader->contacts);

    destroyHashmapConnection(map);
    destroyReader(reader);
    printf("\n--------------------------------------------------------------------------------------------- Fin du programme ----------------------------------------------------------------------------------------------------\n");

    return EXIT_SUCCESS;
}