#include "../../headers/Jeu/Puissance4.h"
#include <stdlib.h>
#include <string.h>

Jeu *creerJeu(ModeDeJeu mode)
{
    Jeu *jeu = (Jeu *)malloc(sizeof(Jeu));
    if (!jeu)
    {
        setErreur(ERR_ERROR_ALLOCATION);
        return NULL;
    }
    jeu->b = creerBoard();
    if (!jeu->b)
    {
        freeJeu(jeu);
        return NULL;
    }
    jeu->mode = mode;
    creerJoueursSelonMode(&jeu);
    if (!jeu->j1 || !jeu->j2)
    {
        setErreur(ERR_ERROR_ALLOCATION);
        freeJeu(jeu);
        return NULL;
    }
    jeu->j1->numero = 1;
    jeu->j2->numero = 2;
    return jeu;
}

Jeu *creerJeuGraphique(ModeDeJeu mode, enum ia_ ia1, enum ia_ ia2)
{
    Jeu *jeu = (Jeu *)malloc(sizeof(Jeu));
    if (!jeu)
    {
        setErreur(ERR_ERROR_ALLOCATION);
        return NULL;
    }
    jeu->b = creerBoard();
    if (!jeu->b)
    {
        freeJeu(jeu);
        return NULL;
    }
    jeu->mode = mode;
    creerJoueursSelonModeGraphique(&jeu, ia1, ia2);
    if (!jeu->j1 || !jeu->j2)
    {
        setErreur(ERR_ERROR_ALLOCATION);
        freeJeu(jeu);
        return NULL;
    }
    jeu->j1->numero = 1;
    jeu->j2->numero = 2;
    return jeu;
}

void creerJoueursSelonModeGraphique(Jeu **jeu, enum ia_ ia1, enum ia_ ia2)
{
    switch ((*jeu)->mode)
    {
    case JvsJ:
        (*jeu)->j1 = creerJoueur("Joueur 1", ia1);
        (*jeu)->j2 = creerJoueur("Joueur 2", ia2);
        break;
    case JvsIA:
        (*jeu)->j1 = creerJoueur("Joueur", ia1);
        (*jeu)->j2 = creerJoueur("IA", ia2);
        break;
    case IAvsIA:
        (*jeu)->j1 = creerJoueur("IA 1", ia1);
        (*jeu)->j2 = creerJoueur("IA 2", ia2);
        break;
    default:
        break;
    }
}

void creerJoueursSelonMode(Jeu **jeu)
{
    switch ((*jeu)->mode)
    {
    case JvsJ:
        (*jeu)->j1 = creerJoueur("Joueur 1", USER);
        (*jeu)->j2 = creerJoueur("Joueur 2", USER);
        break;
    case JvsIA:
        (*jeu)->j1 = creerJoueur("Joueur", USER);
        (*jeu)->j2 = creerJoueur("IA", demandeDifficulteIA());
        break;
    case IAvsIA:
        (*jeu)->j1 = creerJoueur("IA 1", demandeDifficulteIA());
        (*jeu)->j2 = creerJoueur("IA 2", demandeDifficulteIA());
        break;
    default:
        break;
    }
}

IA demandeDifficulteIA()
{
    printf("Entrez la difficulté voulue pour l'IA (FACILE, NORMAL ou DIFFICILE, en majuscule)\n");
    char *diff = (char *)malloc(sizeof(char) * 20);
    fgets(diff, 19, stdin);
    diff[strlen(diff) - 1] = '\0';
    if (!strcmp(diff, "FACILE"))
    {
        free(diff);
        return FACILE;
    }
    else if (!strcmp(diff, "NORMAL"))
    {
        free(diff);
        return NORMAL;
    }
    else if (!strcmp(diff, "DIFFICILE"))
    {
        free(diff);
        return DIFFICILE;
    }
    free(diff);
    printf("Mauvaise entrée, IA initialisée à NORMAL par défaut.\n");
    return NORMAL;
}

void freeJeu(Jeu *jeu)
{
    if (!jeu)
        return;
    if (jeu->b)
        freeBoard(jeu->b);
    if (jeu->j1)
        freeJoueur(jeu->j1);
    if (jeu->j2)
        freeJoueur(jeu->j2);
    free(jeu);
}

Jeu *copyJeu(Jeu *jeu)
{
    Jeu *new = (Jeu *)malloc(sizeof(Jeu));
    if (!new)
    {
        setErreur(ERR_ERROR_ALLOCATION);
        return NULL;
    }
    new->mode = jeu->mode;
    new->b = copyBoard(jeu->b);
    if (!new->b)
    {
        free(new);
        return NULL;
    }
    new->j1 = copyJoueur(jeu->j1);
    if (!new->j1)
    {
        freeJeu(new);
        return NULL;
    }
    new->j2 = copyJoueur(jeu->j2);
    if (!new->j2)
    {
        freeJeu(new);
        return NULL;
    }
    return new;
}