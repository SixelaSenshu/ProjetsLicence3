#include "../../headers/Vue/jeuTextuel.h"
#include <stdlib.h>
#include <unistd.h>

void jouerTexte()
{
    Jeu *j;
    printf("Choisissez le mode de jeu voulu: \n");
    printf("Tapez 1 pour Joueur contre Joueur.\n");
    printf("Tapez 2 pour Joueur contre IA.\n");
    printf("Tapez 3 pour IA contre IA.\n");
    printf("Une mauvaise saisie entraîne le lancement de la partie en mode IA contre IA.\n");
    char *input = (char *)malloc(sizeof(char) * 5);
    if (!input)
    {
        setErreur(ERR_ERROR_ALLOCATION);
        return;
    }
    fgets(input, 4, stdin);
    switch (input[0])
    {
    case '1':
        j = creerJeu(JvsJ);
        break;
    case '2':
        j = creerJeu(JvsIA);
        break;
    case '3':
        j = creerJeu(IAvsIA);
        break;
    default:
        j = creerJeu(IAvsIA);
        break;
    }
    free(input);
    afficherBoard(j->b);
    Joueur *tour = j->j1;
    while (!finJeu(j->b))
    {
        printf("C'est au tour de %s de jouer.\n", tour->name);
        tour->play(j);
        afficherBoard(j->b);
        if (checkVictoire(j->b))
        {
            printf("%s a gagné.\n", tour->name);
            break;
        }
        (tour == j->j1) ? (tour = j->j2) : (tour = j->j1);
        if (tour->ia != USER)
            sleep(1);
    }
    if (!checkVictoire(j->b) && boardComplet(j->b))
        printf("Egalité :/\n");
    freeJeu(j);
}