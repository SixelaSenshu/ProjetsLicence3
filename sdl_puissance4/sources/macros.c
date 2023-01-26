#include "../headers/macros.h"

static enum err erreur;

extern enum err getErreur(){
    return erreur;
}

extern void afficherErreur(){
    switch (getErreur())
    {
    case ERR_ERROR_ALLOCATION:
        fprintf(stderr,"[ERR_ERROR_ALLOCATION] Fin du programme du à un problème d'allocation \n");
        break;
    case ERR_ERROR_INIT_SDL:
        fprintf(stderr,"[ERR_ERROR_INIT_SDL] Fin du programme du à un problème d'initialisation de la SDL \n");
        break;
    case ERR_ERROR_SDL:
        fprintf(stderr,"[ERR_ERROR_SDL] Annulation de la modification du programme du à un problème de la SDL \n");
        break;
    case ERR_ERROR_SDL_BREAK:
        fprintf(stderr,"[ERR_ERROR_SDL_BREAK] Fin du programme du à un problème de la SDL \n");
        break;
    case ERR_FULL:
        fprintf(stderr,"[ERR_FULL] Annulation de certaines modifications du à un choix déja fait ou un champ déja remplis \n");
        break;
    case ERR_INVALID_OPTION:
        fprintf(stderr,"[ERR_INVALID_OPTION] Valeurs mise à défaut pour une partie des paramètres du à une mauvaise saisie ou acces de valeurs \n");
        break;
    case ERR_INVALID_POINTER:
        fprintf(stderr,"[ERR_INVALID_POINTER] Annulation d'une commande du à un problème de valeurs non initialisées \n");
        break;
    case ERR_MODIF:
        fprintf(stderr,"[ERR_MODIF] Fin du programme ou annulation d'une commande du à un problème de modification de valeurs \n");
        break;
    case ERR_NO_ERROR:
        fprintf(stderr,"[ERR_NO_ERROR] Fin du programme : Aucun problème à signaler\n");
        break;
    case ERR_EVENT_SDL:
        fprintf(stderr,"[ERR_EVENT_SDL] Fin du programme ou annulation d'un evenement : Evenement inattendu ou inconnu \n");
        break;
    default:
        break;
    }
}

extern void setErreur(enum err e){
    erreur = e;
    afficherErreur();
}