#include "../../headers/Jeu/Joueur.h"
#include "../../headers/Jeu/IA.h"
#include <stdlib.h>
#include <string.h>

Joueur* creerJoueur (char* name, IA ia) {
    Joueur* joueur = (Joueur*) malloc (sizeof (Joueur));
    if (!joueur) {
        setErreur(ERR_ERROR_ALLOCATION);
        return NULL;
    }
    joueur->name = (char*) malloc (sizeof (char)*(strlen(name)+1)); 
    if (!joueur->name) {
        setErreur(ERR_ERROR_ALLOCATION);
        freeJoueur(joueur);
        return NULL;
    }
    strcpy (joueur->name, name);
    joueur->ia = ia;
    switch (ia) { //j'avais essayé de faire une macro pour ça mais pas réussi :/
        case FACILE:
            joueur->play = jouer_FACILE;
            break;
        case NORMAL:
            joueur->play = jouer_NORMAL;
            break;
        case DIFFICILE:
            joueur->play = jouer_DIFFICILE;
            break;
        case USER:
            joueur->play = jouer_USER;
            break;
        default:
            setErreur(ERR_INVALID_OPTION);
            freeJoueur(joueur);
            return NULL;
    }
    return joueur;
}

void freeJoueur (Joueur* j) {
    if (!j) return;
    if (j->name) free (j->name);
    free (j);
}

Joueur* copyJoueur (Joueur* j) {
    if (!j) return NULL;
    Joueur* new = creerJoueur(j->name, j->ia);
    if (!new) return NULL;
    new->numero = j->numero;
    new->score = j->score;
    new->play = j->play;
    strcpy(new->name, j->name);
    return new;
}