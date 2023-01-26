#include "../../headers/Vue/SDL_palette.h"

extern SDL_Palette_Interface *SDL_Palette_creer()
{
    SDL_Palette_Interface *palette = (SDL_Palette_Interface *)malloc(sizeof(SDL_Palette_Interface));
    if (palette == NULL)
        goto quit;
    palette->bouton = (SDL_Color){150, 150, 150, 255};
    palette->j1 = (SDL_Color){0, 0, 255, 255};
    palette->j2 = (SDL_Color){255, 0, 0, 255};
    palette->mur = (SDL_Color){0, 100, 100, 255};
    palette->texte = (SDL_Color){255, 255, 255, 255};
    palette->casevide = (SDL_Color){200, 200, 200, 255};
    return palette;
quit:
    setErreur(ERR_ERROR_ALLOCATION);
    fprintf(stderr, "[ERREUR] L'instance SDL_Palette n'a pas pu être alloué : Annulation de l'allocation.\n");
    return NULL;
}

extern bool SDL_Palette_detruire(SDL_Palette_Interface *palette)
{
    if (NULL != palette)
    {
        free(palette);
        return true;
    }
    else
    {
        setErreur(ERR_INVALID_POINTER);
        fprintf(stderr, "[ATTENTION] L'instance SDL_Palette n'existe pas : Aucune destruction.\n");
        return false;
    }
}

extern bool SDL_Palette_changeCouleurElement(SDL_Palette_Interface *palette, SDL_Palette_Type type, SDL_Color couleur)
{
    if (palette)
    {
        setErreur(ERR_INVALID_POINTER);
        fprintf(stderr, "[ATTENTION] L'instance SDL_Palette n'existe pas : Aucune destruction.\n");
        return false;
    }
    switch (type)
    {
    case SDL_PALET_TYPE_BACKGROUND:
        palette->mur = couleur;
        break;
    case SDL_PALET_TYPE_PLAYER1:
        palette->j1 = couleur;
        break;
    case SDL_PALET_TYPE_PLAYER2:
        palette->j2 = couleur;
        break;
    case SDL_PALET_TYPE_TEXT:
        palette->texte = couleur;
        break;
    case SDL_PALET_TYPE_BUTTON:
        palette->bouton = couleur;
        break;
    case SDL_PALET_TYPE_JETONVIDE:
        palette->casevide = couleur;
        break;
    }
    return true;
}

extern SDL_Color SDL_Palette_donneCouleurElement(SDL_Palette_Interface *palette, SDL_Palette_Type type)
{
    SDL_Color recup = (SDL_Color){0, 0, 0, 255};
    if (!palette)
    {
        setErreur(ERR_INVALID_POINTER);
        fprintf(stderr, "[ATTENTION] L'instance SDL_Palette_Interface n'existe pas : Aucune destruction.\n");
        return (SDL_Color){0, 0, 0, 0};
    }
    switch (type)
    {
    case SDL_PALET_TYPE_BACKGROUND:
        recup = palette->mur;
        break;
    case SDL_PALET_TYPE_PLAYER1:
        recup = palette->j1;
        break;
    case SDL_PALET_TYPE_PLAYER2:
        recup = palette->j2;
        break;
    case SDL_PALET_TYPE_TEXT:
        recup = palette->texte;
        break;
    case SDL_PALET_TYPE_BUTTON:
        recup = palette->bouton;
        break;
    case SDL_PALET_TYPE_JETONVIDE:
        recup = palette->casevide;
    }
    return recup;
}

extern bool SDL_Palette_appliqueCouleurElement(SDL_Palette_Interface *palette, SDL_Palette_Type type, SDL_Renderer *fenetre)
{
    if (NULL == fenetre)
    {
    }
    else
    {
        SDL_Color couleur = SDL_Palette_donneCouleurElement(palette, type);
        if (0 != SDL_SetRenderDrawColor(fenetre, couleur.r, couleur.g, couleur.b, couleur.a))
            goto quit;
        return true;
    }
quit:
    fprintf(stderr, "[ERREUR] Problème d'application de la SDL_Couleur invalide : Génération de la la couleur annulé.\n");
    setErreur(ERR_INVALID_OPTION);
    return false;
}