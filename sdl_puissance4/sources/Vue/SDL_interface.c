#include "../../headers/Vue/SDL_interface.h"

///////////////////// Fonctions internes

static bool estDansCercle(int, int, int, int);
static bool SDL_Interface_dessinerCercleDansGrille(SDL_Interface **interface, SDL_Rect *coord, SDL_PixelFormat *format, SDL_Color couleur);
static bool SDL_Interface_dessinerCercles(SDL_Interface **interface, SDL_Rect *coord);
static bool SDL_interface_dessinerListeBouttons(SDL_Interface *interface, SDL_Rect *coord, int choice);
static bool SDL_Interface_dessinerInfoBoutons(SDL_Interface *interface, SDL_Rect *coord);
static bool SDL_Interface_dessinerInfoJouer(SDL_Interface *interface, SDL_Rect *coord);

///////////////////// Function Allocation et Désallocation de mémoire

extern SDL_Interface *SDL_Interface_Creer(unsigned hauteur, unsigned largeur)
{
    SDL_Interface *interface = (SDL_Interface *)malloc(sizeof(SDL_Interface));
    if (NULL == interface)
    {
        perror("[ERROR] Problème de génération de la fenètre : Fin du programme.\n");
        setErreur(ERR_ERROR_ALLOCATION);
        goto quit;
    }
    // Initialisation des périphériques
    if (0 != SDL_InitSubSystem(SDL_INIT_EVENTS))
    {
        setErreur(ERR_ERROR_INIT_SDL);
        fprintf(stderr, "[ERREUR]%s", SDL_GetError());
        goto quit;
    }
    if (0 != SDL_InitSubSystem(SDL_INIT_VIDEO))
    {
        setErreur(ERR_ERROR_INIT_SDL);
        fprintf(stderr, "[ERREUR]%s", SDL_GetError());
        goto quit;
    }
    // Initialisation des paramètres
    interface->hauteur = hauteur;
    interface->largeur = largeur;

    interface->paramwindow = SDL_INTERFACE_WINDOW_SHOW;
    interface->mode_jeu = JvsJ;
    interface->ia1 = USER;
    interface->ia2 = USER;
    if (false == SDL_Interface_creerFenetreSDL(interface, "SDL"))
        return NULL;
    if (false == SDL_Interface_creerRenderer(interface))
        return NULL;
    if (false == SDL_Interface_creerTexture(interface))
        return NULL;
    if (false == SDL_Interface_creerPalette(interface))
        return NULL;
    if (false == SDL_Interface_Coord_creer(interface))
        return NULL;
    if (false == SDL_Interface_creerJeu(interface))
        return NULL;
    SDL_Interface_actualiserCoordonees(interface);
    return interface;
quit:
    SDL_Interface_detruire_(&interface);
    return NULL;
}

extern bool SDL_Interface_creerFenetreSDL(SDL_Interface *interface, char *titre)
{
    if (NULL == interface)
    {
        setErreur(ERR_INVALID_POINTER);
        fprintf(stderr, "[ATTENTION] Pas de struct 'SDL_Interface' initialisée : Annulation de la génération de fenêtre\n");
        return false;
    }
    // Création de la fenêtre
    SDL_Window *tmp = SDL_CreateWindow(titre, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, interface->largeur, interface->hauteur, SDL_WINDOW_RESIZABLE);
    if (!tmp)
        goto quit;
    interface->fenetre = tmp;
    SDL_SetWindowMinimumSize(interface->fenetre, 500, 500);
    SDL_SetWindowMaximumSize(interface->fenetre, 1400, 1000);
    SDL_SetWindowBordered(tmp, true);
    return true;
quit:
    setErreur(ERR_ERROR_ALLOCATION);
    fprintf(stderr, "[ERREUR] Problème d'allocation d'une instance SDL_Window : Fin du programme\n");
    SDL_Interface_detruire_(&interface);
    return false;
}

extern bool SDL_Interface_creerRenderer(SDL_Interface *interface)
{
    if (!interface)
    {
        SDL_INTERFACE_MACRO_NO_EXIST_INSTANCE(ERR_INVALID_POINTER, stderr, "[ATTENTION] Pas de struct 'SDL_interface' initialisée : Annulation de la génération du renderer de la fenêtre\n", false);
    }
    // Création du renderer de la fenêtre.
    SDL_Renderer *renderer = SDL_CreateRenderer(interface->fenetre, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        goto quit;
    interface->renderer = renderer;
    return true;
quit:
    setErreur(ERR_ERROR_ALLOCATION);
    fprintf(stderr, "[ERREUR] Problème d'allocation d'un renderer : Fin du programme\n");
    fprintf(stderr, "[SDL]%s\n", SDL_GetError());
    SDL_Interface_detruire_(&interface);
    return false;
}

extern bool SDL_Interface_creerTexture(SDL_Interface *interface)
{
    if (NULL == interface)
    {
        SDL_INTERFACE_MACRO_NO_EXIST_INSTANCE(ERR_INVALID_POINTER, stderr, "[ATTENTION] Pas de struct 'SDL_interface' initialisée : Annulation de la génération des textures de la fenêtre\n", false);
    }
    /////// Initialisation de la matrice de texture
    SDL_Texture *texture = SDL_CreateTexture(interface->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, interface->largeur, interface->hauteur);
    if (texture == NULL)
        goto quit;
    else
        interface->texture = texture;
    return true;
quit:
    setErreur(ERR_ERROR_ALLOCATION);
    fprintf(stderr, "[ERREUR] Problème d'allocation d'une texture : Fin du programme\n");
    fprintf(stderr, "[SDL]%s\n", SDL_GetError());
    SDL_Interface_detruire_(&interface);
    return false;
}

extern bool SDL_Interface_creerPalette(SDL_Interface *interface)
{
    if (NULL == interface)
    {
        SDL_INTERFACE_MACRO_NO_EXIST_INSTANCE(ERR_INVALID_POINTER, stderr, "[ATTENTION] Pas de struct 'SDL_interface' initialisée : Annulation de la génération de la palette de la fenêtre\n", false);
    }
    /////// Initialisation de la matrice de texture
    SDL_Palette_Interface *palette = SDL_Palette_creer();
    if (palette == NULL)
        goto quit;
    else
        interface->couleurs = palette;
    return true;
quit:
    setErreur(ERR_ERROR_ALLOCATION);
    fprintf(stderr, "[ERREUR] Problème d'allocation d'une palette : Fin du programme\n");
    SDL_Interface_detruire_(&interface);
    return false;
}

extern bool SDL_Interface_creerJeu(SDL_Interface *interface)
{
    if (NULL == interface)
    {
        SDL_INTERFACE_MACRO_NO_EXIST_INSTANCE(ERR_INVALID_POINTER, stderr, "[ATTENTION] Pas de struct 'SDL_interface' initialisée : Annulation de la génération du jeu\n", false);
    }
    Jeu *jeu = creerJeuGraphique(interface->mode_jeu, interface->ia1, interface->ia2);
    if (jeu == NULL)
        goto quit;
    else
        interface->jeu = jeu;
    return true;
quit:
    setErreur(ERR_ERROR_ALLOCATION);
    fprintf(stderr, "[ERREUR] Problème d'allocation d'un jeu\n");
    SDL_Interface_detruire_(&interface);
    return false;
}

extern bool SDL_Interface_modifierJeu(SDL_Interface *interface, ModeDeJeu mode, IA ia1, IA ia2)
{
    if (NULL == interface)
    {
        SDL_INTERFACE_MACRO_NO_EXIST_INSTANCE(ERR_INVALID_POINTER, stderr, "[ATTENTION] Pas de struct 'SDL_interface' initialisée : Annulation de la génération du jeu\n", false);
    }
    interface->ia1 = ia1;
    interface->ia2 = ia2;
    interface->mode_jeu = mode;
    return true;
}

extern bool SDL_Interface_Coord_creer(SDL_Interface *interface)
{
    if (NULL == interface)
    {
        SDL_INTERFACE_MACRO_NO_EXIST_INSTANCE(ERR_INVALID_POINTER, stderr, "[ATTENTION] Pas de struct 'SDL_interface' initialisée : Annulation de la génération des coordonées de la fenêtre\n", false);
    }
    SDL_Interface_Coord *coords;
    SDL_Rect *boutons;
    SDL_Rect *grille;
    SDL_Rect *info;
    coords = (SDL_Interface_Coord *)malloc(sizeof(SDL_Interface_Coord));
    if (!coords)
    {
        fprintf(stderr, "[ERREUR] Problème d'allocation de la liste des coordonées (SDL_Interface_Coord)\n");
        goto quit;
    }
    boutons = (SDL_Rect *)malloc(sizeof(SDL_Rect));
    if (!boutons)
    {
        fprintf(stderr, "[ERREUR] Problème d'allocation de la liste des coordonées (SDL_Rect)\n");
        goto quit;
    }
    grille = (SDL_Rect *)malloc(sizeof(SDL_Rect));
    if (!grille)
    {
        fprintf(stderr, "[ERREUR] Problème d'allocation de la liste des coordonées (SDL_Rect)\n");
        goto quit;
    }
    info = (SDL_Rect *)malloc(sizeof(SDL_Rect));
    if (!info)
    {
        fprintf(stderr, "[ERREUR] Problème d'allocation de la liste des coordonées (SDL_Rect)\n");
        goto quit;
    }
    coords->boutons = boutons;
    coords->grille = grille;
    coords->info = info;
    coords->nbPos = 3;
    interface->coordonnes = coords;
    return true;
quit:
    setErreur(ERR_ERROR_ALLOCATION);
    SDL_Interface_detruire_(&interface);
    return false;
}

extern bool SDL_Interface_Coord_Detruire(SDL_Interface *interface)
{
    if (NULL == interface)
    {
        SDL_INTERFACE_MACRO_NO_EXIST_INSTANCE(ERR_INVALID_POINTER, stderr, "[ATTENTION] Pas de struct 'SDL_interface' initialisée : Annulation de la destruction des coordonées de la fenêtre\n", false);
    }
    SDL_Interface_Coord *coord = interface->coordonnes;
    if (NULL != coord)
    {
        if (NULL != coord->boutons)
            free(coord->boutons);
        if (NULL != coord->grille)
            free(coord->grille);
        if (NULL != coord->info)
            free(coord->info);
    }
    free(coord);
    return true;
}

extern bool SDL_Interface_detruire_(SDL_Interface **interface)
{
    if (NULL == *interface)
    {
        SDL_INTERFACE_MACRO_NO_EXIST_INSTANCE(ERR_INVALID_POINTER, stderr, "[ATTENTION] Pas de struct 'SDL_interface' initialisée : Annulation de la destruction de la fenêtre\n", false);
    }
    else
    {
        /* On detruit d'abord les composantes de la SDL dans l'ordre suivant :
        SDL_Texture, SDL_Renderer puis SDL_Window et on free la nouvelle structure*/
        if (NULL != (*interface)->texture)
            SDL_DestroyTexture((*interface)->texture);
        if (NULL != (*interface)->renderer)
            SDL_DestroyRenderer((*interface)->renderer);
        if (NULL != (*interface)->fenetre)
            SDL_DestroyWindow((*interface)->fenetre);
        if (NULL != (*interface)->couleurs)
            SDL_Palette_detruire((*interface)->couleurs);
        if (NULL != (*interface)->coordonnes)
            SDL_Interface_Coord_Detruire((*interface));
        if (NULL != (*interface)->jeu)
            freeJeu((*interface)->jeu);
        free(*interface);
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
        SDL_QuitSubSystem(SDL_INIT_EVENTS);
    }
    return true;
}

///////////////////// Fonctions utilitaires pour la SDL_Interface

extern bool SDL_Interface_changerModeVisuel(SDL_Interface *interface, SDL_Interface_Param_Window itf_option)
{
    if (NULL == interface)
    {
        SDL_INTERFACE_MACRO_NO_EXIST_INSTANCE(ERR_INVALID_POINTER, stderr, "[ATTENTION] Pas de struct 'SDL_interface' initialisée : Annulation de la modification visuelle de la fenêtre\n", false);
    }

    SDL_GetWindowSize(interface->fenetre, &interface->largeur, &interface->hauteur);
    switch (itf_option)
    {
    case SDL_INTERFACE_WINDOW_MAX:
        SDL_MaximizeWindow(interface->fenetre);
        break;
    case SDL_INTERFACE_WINDOW_MIN:
        SDL_MinimizeWindow(interface->fenetre);
        break;
    case SDL_INTERFACE_WINDOW_RESTORE:
        SDL_RestoreWindow(interface->fenetre);
        break;
    case SDL_INTERFACE_WINDOW_SHOW:
        SDL_ShowWindow(interface->fenetre);
        break;
    default:
        fprintf(stderr, "[ATTENTION] Paramètre de modification de la fenêtre inconnu : modification par défaut\n");
        setErreur(ERR_INVALID_OPTION);
        break;
    }
    return true;
}

extern bool SDL_Interface_nettoyerEcran(SDL_Interface *interface)
{
    if (NULL == interface)
    {
        SDL_INTERFACE_MACRO_NO_EXIST_INSTANCE(ERR_INVALID_POINTER, stderr, "[ATTENTION] Pas de struct 'SDL_interface' initialisée : Annulation du nettoyage de la fenêtre\n", false);
    }
    /** On définie la couleur qui remplira l'ecran et on le nettoie
     * avant de le mettre le renderer à jour*/
    if (false == SDL_Palette_appliqueCouleurElement(interface->couleurs, SDL_PALET_TYPE_BACKGROUND, interface->renderer))
        goto quit;
    if (0 != SDL_RenderClear(interface->renderer))
    {
        setErreur(ERR_ERROR_SDL);
        fprintf(stderr, "[SDL] %s\n", SDL_GetError());
        goto quit;
    }
    SDL_RenderPresent(interface->renderer);
    return true;
quit:
    SDL_Interface_detruire_(&interface);
    return false;
}

extern bool SDL_Interface_mettreImage(SDL_Interface *interface, const char filename[], SDL_Rect *rect)
{
    if (NULL == interface)
    {
        SDL_INTERFACE_MACRO_NO_EXIST_INSTANCE(ERR_INVALID_POINTER, stderr, "[ATTENTION] Pas de struct 'SDL_interface' initialisée : Annulation du nettoyage de la fenêtre\n", false);
    }
    SDL_Surface *surface = NULL;
    SDL_Texture *tmp = NULL;
    SDL_Rect r;
    SDL_Rect img;
    surface = SDL_LoadBMP(filename);
    if (NULL == surface)
    {
        setErreur(ERR_ERROR_INIT_SDL);
        goto quit;
    }
    tmp = SDL_CreateTextureFromSurface(interface->renderer, surface);
    if (NULL == tmp)
    {
        setErreur(ERR_ERROR_INIT_SDL);
        goto quit;
    }
    r = (SDL_Rect){rect->x, rect->y, rect->w, rect->h};
    img = (SDL_Rect){0, 0, surface->w, surface->h};
    if (0 != SDL_RenderCopy(interface->renderer, tmp, &img, &r))
    {
        setErreur(ERR_ERROR_SDL_BREAK);
        goto quit;
    }
    SDL_RenderPresent(interface->renderer);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(tmp);
    return true;
quit:
    fprintf(stderr, "[SDL]%s\n", SDL_GetError());
    fprintf(stderr, "[ERREUR] Problème d'affichage d'image\n");
    if (NULL != surface)
        SDL_FreeSurface(surface);
    if (NULL != tmp)
        SDL_DestroyTexture(tmp);
    return false;
}

static bool estDansCercle(int x, int y, int h, int l)
{
    /* Verifie si le point est dans le cercle*/
    int r = (h < l) ? (h / 2) + (h - l) : (l / 2) + (l - h);
    if (sqrt(pow((x - h / 2), 2) + pow((y - l / 2), 2)) < r * 0.90)
        return true;
    return false;
}

///////////////////// Fonctions d'affichage de la grille

static bool SDL_Interface_dessinerCercleDansGrille(SDL_Interface **interface, SDL_Rect *coord, SDL_PixelFormat *format, SDL_Color couleur)
{
    /* Dessine un cercle pour une position, une hauteur et une largeur donnée dans une
    interface. On met à jour la texture par la suite*/
    int i, j, r = (coord->h < coord->w) ? coord->h : coord->w;
    Uint32 pixels[coord->h][coord->w];

    /* Mise à jour de la texture*/
    for (j = 0; j < coord->w; j++)
    { // On créer la matrice de cercle
        for (i = 0; i < coord->h; i++)
        { // On calcule si le point est dans le cercle
            if (estDansCercle(i, j, r, r))
            {
                // Si le point est dans le cercle, on dessine avec la couleur chosie
                pixels[i][j] = SDL_MapRGBA(format, couleur.r, couleur.g, couleur.b, couleur.a);
            }
            else
            {
                // Sinon on le met dans la couleur du mur
                SDL_Color c = SDL_Palette_donneCouleurElement((*interface)->couleurs, SDL_PALET_TYPE_BACKGROUND);
                pixels[i][j] = SDL_MapRGBA(format, c.r, c.g, c.b, c.a);
            }
        }
    }
    // On met à jour la texture
    if (0 != SDL_UpdateTexture((*interface)->texture, coord, pixels, coord->w * sizeof(Uint32)))
    {
        fprintf(stderr, "[SDL]%s", SDL_GetError());
        goto quit;
    }
    return true;
quit:
    setErreur(ERR_ERROR_INIT_SDL);
    return false;
}

static bool SDL_Interface_dessinerCercles(SDL_Interface **interface, SDL_Rect *coord)
{
    /* Générer la matrice de cercle à partir de SDL_interface_dessinerCercleDansGrille
    avec les même types de paramètres que celle-ci*/
    int new_x, new_y;
    int new_h = coord->h / SDL_INTERFACE_NB_LIGNE, new_w = coord->w / SDL_INTERFACE_NB_COLONNE;
    // On créer le format pour le pixel soit RGB (Uint32)
    SDL_Color couleur;
    SDL_Rect des;
    SDL_PixelFormat *format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
    if (!format)
    {
        fprintf(stderr, "[ERREUR] Problème d'allocation du format: Annulation de l'allocation du format\n");
        setErreur(ERR_ERROR_ALLOCATION);
        return false;
    }
    /*Pour chaque carré de la surface de la grille, on va dessiner un cercle, le mettre à jour puis continuer
    jusqu'a avoir fait le 7 colonnes et les 6 lignes*/

    for (int i = 0; i < SDL_INTERFACE_NB_LIGNE; i++)
    {
        new_y = (i * new_h) + coord->y;
        for (int j = 0; j < SDL_INTERFACE_NB_COLONNE; j++)
        {
            new_x = (j * new_w) + coord->x;
            if ((*interface)->jeu->b->jetons[i][j] == 1)
            {
                couleur = SDL_Palette_donneCouleurElement((*interface)->couleurs, SDL_PALET_TYPE_PLAYER1);
            }
            else if ((*interface)->jeu->b->jetons[i][j] == 2)
            {
                couleur = SDL_Palette_donneCouleurElement((*interface)->couleurs, SDL_PALET_TYPE_PLAYER2);
            }
            else
            {
                couleur = SDL_Palette_donneCouleurElement((*interface)->couleurs, SDL_PALET_TYPE_JETONVIDE);
            }
            des = (SDL_Rect){new_x, new_y, new_w, new_h};
            if (false == SDL_Interface_dessinerCercleDansGrille(interface, &des, format, couleur))
            {
                SDL_FreeFormat(format);
                return false;
            }
        }
    }
    SDL_FreeFormat(format);
    return true;
}

extern bool SDL_Interface_dessinerGrille(SDL_Interface **interface, SDL_Rect *coord)
{
    if (NULL == interface)
    {
        SDL_INTERFACE_MACRO_NO_EXIST_INSTANCE(ERR_INVALID_POINTER, stderr, "[WARNING] Pas de struct 'SDL_interface' initialisée : Annulation de la génération de la grille de la fenêtre\n", false);
    }
    if (false == SDL_Interface_dessinerCercles(interface, coord))
    {
        fprintf(stderr, "[ERROR] Erreur de modification de la texture : Annulation de la modification \n");
        goto quit;
    }
    if (0 != SDL_RenderCopy((*interface)->renderer, (*interface)->texture, coord, coord))
    {
        fprintf(stderr, "[SDL]%s", SDL_GetError());
        goto quit;
    }
    SDL_RenderPresent((*interface)->renderer);
    return true;
quit:
    setErreur(ERR_ERROR_SDL);
    return false;
}

///////////////////// Fonctions d'affichage des buttons

static bool SDL_interface_dessinerListeBouttons(SDL_Interface *interface, SDL_Rect *coord, int choice)
{
    int size_w = (coord->w / 3);
    SDL_Rect rect = (SDL_Rect){coord->x, coord->y, size_w, coord->h};
    SDL_Rect rect2 = (SDL_Rect){coord->x + size_w, coord->y, size_w, coord->h};
    SDL_Rect rect3 = (SDL_Rect){coord->x + (size_w * 2), coord->y, size_w, coord->h};
    if (choice == 0)
    { // Dans le choix ou on affiche les mode de jeu
        if (false == SDL_Interface_mettreImage(interface, "./images/jvsj.bmp", &rect))
            goto quit;
        if (false == SDL_Interface_mettreImage(interface, "./images/iavsj.bmp", &rect2))
            goto quit;
        if (false == SDL_Interface_mettreImage(interface, "./images/iavsia.bmp", &rect3))
            goto quit;
    }
    else
    { // Dans le cas ou on affiche les différents niveaux de l'IA

        if (false == SDL_Interface_mettreImage(interface, "./images/easy.bmp", &rect))
            goto quit;
        if (false == SDL_Interface_mettreImage(interface, "./images/medium.bmp", &rect2))
            goto quit;
        if (false == SDL_Interface_mettreImage(interface, "./images/hard.bmp", &rect3))
            goto quit;
    }
    return true;
quit:
    return false;
}

extern bool SDL_Interface_dessinerBouttons(SDL_Interface **interface, SDL_Rect *coord, int choice)
{
    if (NULL == interface)
    {
        SDL_INTERFACE_MACRO_NO_EXIST_INSTANCE(ERR_INVALID_POINTER, stderr, "[ATTENTION] Pas de struct 'SDL_interface' initialisée : Annulation de la génération des boutons de la fenêtre\n", false);
    }
    if (false == SDL_Palette_appliqueCouleurElement((*interface)->couleurs, SDL_PALET_TYPE_BUTTON, (*interface)->renderer))
    {
        goto quit;
    }
    if (false == SDL_interface_dessinerListeBouttons(*interface, coord, choice))
    {
        goto quit;
    }
    SDL_RenderPresent((*interface)->renderer);
    return true;
quit:
    return false;
}

///////////////////// Fonctions d'affichage des informations

static bool SDL_Interface_dessinerInfoBoutons(SDL_Interface *interface, SDL_Rect *coord)
{
    SDL_Rect rect1 = (SDL_Rect){coord->x, coord->y, coord->w, coord->h / 2};
    SDL_Rect rect2 = (SDL_Rect){coord->x, coord->y + (coord->h / 2), coord->w, (coord->h / 2)};
    if (false == SDL_Interface_mettreImage(interface, "./images/play.bmp", &rect1))
        goto quit;
    if (false == SDL_Interface_mettreImage(interface, "./images/restart.bmp", &rect2))
        goto quit;

    return true;
quit:
    return false;
}

static bool SDL_Interface_dessinerInfoJouer(SDL_Interface *interface, SDL_Rect *coord)
{
    short jetonx = interface->jeu->b->dernierJeton[0];
    short jetony = interface->jeu->b->dernierJeton[1];

    if (jetonx == -1 && jetonx == -1)
    {
        if (false == SDL_Interface_mettreImage(interface, "./images/empty.bmp", coord))
            goto quit;
    }
    else if (interface->jeu->b->jetons[jetonx][jetony] == 1)
    {
        if (false == SDL_Interface_mettreImage(interface, "./images/blue.bmp", coord))
            goto quit;
    }
    else if (interface->jeu->b->jetons[jetonx][jetony] == 2)
    {
        if (false == SDL_Interface_mettreImage(interface, "./images/red.bmp", coord))
            goto quit;
    }

    return true;
quit:
    return false;
}

extern bool SDL_Interface_dessinerInfo(SDL_Interface **interface, SDL_Rect *coord)
{
    SDL_Rect rect1 = (SDL_Rect){coord->x, (coord->y + (coord->h / 3)), coord->w, (coord->h - (coord->h / 3))};
    SDL_Rect rect2 = (SDL_Rect){coord->x, coord->y, coord->w, (coord->h / 3)};
    if (NULL == interface)
    {
        SDL_INTERFACE_MACRO_NO_EXIST_INSTANCE(ERR_INVALID_POINTER, stderr, "[WARNING] Pas de struct 'SDL_interface' initialisée : Annulation de l'actualisation de la fenêtre\n", false);
    }
    if (false == SDL_Palette_appliqueCouleurElement((*interface)->couleurs, SDL_PALET_TYPE_BUTTON, (*interface)->renderer))
        goto quit;
    if (false == SDL_Interface_dessinerInfoBoutons((*interface), &rect1))
        goto quit;
    if (false == SDL_Interface_dessinerInfoJouer((*interface), &rect2))
        goto quit;
    if (0 != SDL_RenderCopy((*interface)->renderer, (*interface)->texture, &coord, &coord))

    {
        fprintf(stderr, "[SDL]%s\n", SDL_GetError());
        goto quit;
    }
    // SDL_RenderPresent((*interface)->renderer);
    return true;
quit:
    return false;
}

extern bool SDL_Interface_dessinerResultat(SDL_Interface *interface, SDL_Rect *coord)
{
    Jeu *j = interface->jeu;
    SDL_Rect rect = (SDL_Rect){coord->x, coord->y, coord->w, coord->h};
    short jetonx = interface->jeu->b->dernierJeton[0];
    short jetony = interface->jeu->b->dernierJeton[1];
    if (NULL == interface)
    {
        SDL_INTERFACE_MACRO_NO_EXIST_INSTANCE(ERR_INVALID_POINTER, stderr, "[WARNING] Pas de struct 'SDL_interface' initialisée : Annulation de l'actualisation de la fenêtre\n", false);
    }
    if (!checkVictoire(j->b) && boardComplet(j->b))
    {
        if (false == SDL_Interface_mettreImage(interface, "./images/win.bmp", &rect))
            goto quit;
        return 2;
    }
    else if (checkVictoire(j->b))
    {
        if (j->b->jetons[jetonx][jetony] == 1)
        {
            if (false == SDL_Interface_mettreImage(interface, "./images/winj1.bmp", &rect))
                goto quit;
        }
        else if (j->b->jetons[jetonx][jetony] == 2)
        {
            if (false == SDL_Interface_mettreImage(interface, "./images/winj2.bmp", &rect))
                goto quit;
        }
    }
    SDL_RenderPresent(interface->renderer);
    return true;
quit:
    return false;
}
///////////////////////////////////// Fonction généralistes

extern bool SDL_Interface_actualiserCoordonees(SDL_Interface *itf)
{
    if (NULL == itf)
    {
        SDL_INTERFACE_MACRO_NO_EXIST_INSTANCE(ERR_INVALID_POINTER, stderr, "[WARNING] Pas de struct 'SDL_interface' initialisée : Annulation de l'actualisation des coordonées de la fenêtre\n", false);
    }
    SDL_GetWindowSize(itf->fenetre, &itf->largeur, &itf->hauteur);
    int splith10 = itf->hauteur / 10, splitl2 = itf->largeur / 2, splith5 = itf->hauteur / 10;
    /// boutons
    itf->coordonnes->boutons->x = 0;
    itf->coordonnes->boutons->y = 0;
    itf->coordonnes->boutons->h = splith10 * 2;
    itf->coordonnes->boutons->w = itf->largeur;
    /// grille
    itf->coordonnes->grille->x = 0;
    itf->coordonnes->grille->y = splith10 * 2;
    itf->coordonnes->grille->h = itf->hauteur - splith10 * 2;
    itf->coordonnes->grille->w = (itf->largeur / 5) * 4;
    /// coordonnes
    itf->coordonnes->info->x = (itf->largeur) - (itf->largeur / 5);
    itf->coordonnes->info->y = splith10 * 2;
    itf->coordonnes->info->h = itf->hauteur - splith10 * 2;
    itf->coordonnes->info->w = (itf->largeur / 5);
    return true;
}

extern bool SDL_Interface_actualiserTexture(SDL_Interface *itf, SDL_Interface_Param_Window mode, int choice)
{
    if (NULL == itf)
    {
        SDL_INTERFACE_MACRO_NO_EXIST_INSTANCE(ERR_INVALID_POINTER, stderr, "[WARNING] Pas de struct 'SDL_interface' initialisée : Annulation de l'actualisation de la fenêtre\n", false);
    }
    Jeu *j = itf->jeu;
    if (false == SDL_Interface_changerModeVisuel(itf, mode))
        return false;
    if (false == SDL_Interface_actualiserCoordonees(itf))
        return false;
    if (false == SDL_Interface_nettoyerEcran(itf))
        return false;
    if (false == SDL_Interface_dessinerBouttons(&itf, itf->coordonnes->boutons, choice))
        return false;
    if (false == SDL_Interface_dessinerInfo(&itf, itf->coordonnes->info))
        return false;
    if ((!checkVictoire(j->b) && boardComplet(j->b)) || checkVictoire(j->b))
    {
        if (false == SDL_Interface_dessinerResultat(itf, itf->coordonnes->grille))
            return false;
    }
    else
    {
        if (false == SDL_Interface_dessinerGrille(&itf, itf->coordonnes->grille))
            return false;
    }
    return true;
}

extern bool SDL_Interface_actualiserCoup(SDL_Interface *itf)
{
    if (NULL == itf)
    {
        SDL_INTERFACE_MACRO_NO_EXIST_INSTANCE(ERR_INVALID_POINTER, stderr, "[WARNING] Pas de struct 'SDL_interface' initialisée : Annulation de l'actualisation de la fenêtre\n", false);
    }
    Jeu *j = itf->jeu;
    if (false == SDL_Interface_actualiserCoordonees(itf))
        return false;
    if (false == SDL_Interface_dessinerInfo(&itf, itf->coordonnes->info))
        return false;
    if ((!checkVictoire(j->b) && boardComplet(j->b)) || checkVictoire(j->b))
    {
        if (false == SDL_Interface_dessinerResultat(itf, itf->coordonnes->grille))
            return false;
    }
    else
    {
        if (false == SDL_Interface_dessinerGrille(&itf, itf->coordonnes->grille))
            return false;
    }
    return true;
}

extern SDL_Interface *startSDL(char *titre, int hauteur, int largeur)
{
    printf("---------------------------------------------------------------------------------------- Lancement de la SDL -------------------------------------------------------------------------------------------------------\n");
    SDL_Interface *interface = SDL_Interface_Creer(hauteur, largeur);
    if (interface != NULL)
    {
        if (false == SDL_Interface_actualiserTexture(interface, SDL_INTERFACE_WINDOW_SHOW, 0))
        {
            return NULL;
        }
        return interface;
    }
    return NULL;
}
