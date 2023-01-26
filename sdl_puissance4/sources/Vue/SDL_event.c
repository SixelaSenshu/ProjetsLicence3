#include "../../headers/Vue/SDL_event.h"

static int stop = 0;
static bool isRunning = false;

//////////////////////////////// Fonctions internes

static int SDL_Event_Interface_event_souris(SDL_Event_Interface *evt_itf);
static int SDL_Event_Interface_event_clavier(SDL_Event_Interface *evt_itf);
static int SDL_Event_Interface_event_fenetre(SDL_Event_Interface *evt_itf);
static int SDL_Event_Interface_checkZoneInfo(SDL_Event_Interface *evt_itf, Uint32 y);
static void SDL_Event_Interface_checkZoneGrille(SDL_Event_Interface *evt_itf, Uint32 x);
static void SDL_Event_Interface_checkZoneBoutons(SDL_Event_Interface *evt_itf, Uint32 x);
static int SDL_Event_Interface_jouerCoup(SDL_Event_Interface *evt_itf, int i);

//////////////////////////////// Fonction externes

extern SDL_Event_Interface *SDL_Event_Interface_creer(SDL_Interface *interface)
{
    SDL_Event_Interface *eventitf;
    SDL_Event *event;

    eventitf = (SDL_Event_Interface *)malloc(sizeof(SDL_Event_Interface));
    if (eventitf == NULL)
        goto quit;

    if (interface == NULL)
        goto quit;
    eventitf->fenetre = interface;

    event = (SDL_Event *)malloc(sizeof(SDL_Event));
    if (event == NULL)
        goto quit;

    eventitf->choice = 0;
    eventitf->event = event;
    srandom(time(NULL));
    return eventitf;
quit:
    if (event != NULL)
        free(event);
    return NULL;
}

extern void SDL_Event_Interface_detruire(SDL_Event_Interface *evt_itf)
{
    SDL_Interface *itf = evt_itf->fenetre;
    SDL_Event *evt = evt_itf->event;
    if (evt != NULL)
        free(evt);
    if (itf != NULL)
        SDL_Interface_detruire_(&itf);
    if (evt_itf != NULL)
        free(evt_itf);
}

extern void SDL_Event_Interface_attendre(SDL_Event_Interface *evt_itf)
{
    SDL_Event *evt = evt_itf->event;
    bool state = true;
    while (1)
    {
        if (!state)
            break;
        // Tant qu'il y a des évenements
        while (SDL_PollEvent(evt))
        {
            switch (evt->type)
            {
            case SDL_WINDOWEVENT: // Événement de la fenêtre
                if (0 != SDL_Event_Interface_event_fenetre(evt_itf))
                {
                    state = false;
                }
                break;
            case SDL_KEYUP: // Événement de relâchement d'une touche clavier
                if (0 != SDL_Event_Interface_event_clavier(evt_itf))
                {
                    state = false;
                }
                break;
            case SDL_MOUSEBUTTONUP: // Évenement de pression sur le bouton de la souris
                Uint32 x = evt->button.x, y = evt->button.y;
                if (0 != SDL_Event_Interface_event_souris(evt_itf))
                    state = false;
                Jeu *j = evt_itf->fenetre->jeu;
                Joueur *tour = j->j2;
                if (evt_itf->fenetre->mode_jeu == IAvsIA && stop && isRunning && 1 == SDL_Event_Interface_donneZoneClic(evt_itf, x, y))
                {

                    if (evt_itf->joueur == 0)
                        tour = j->j1;
                    if (!finJeu(j->b) && j)
                    {
                        tour->play(j);
                        if (false == SDL_Interface_actualiserCoup(evt_itf->fenetre))
                            state = false;
                        if (checkVictoire(j->b))
                        {
                            if (2 == SDL_Event_Interface_donneZoneClic(evt_itf, x, y))
                            {
                                if (!SDL_Interface_dessinerResultat(evt_itf->fenetre, evt_itf->fenetre->coordonnes->grille))
                                    state = false;
                            }
                            else if (2 != SDL_Event_Interface_donneZoneClic(evt_itf, x, y))
                            {
                                if (!SDL_Interface_dessinerGrille(&evt_itf->fenetre, evt_itf->fenetre->coordonnes->grille))
                                    state = false;
                            }
                            isRunning = false;
                            stop = 0;
                        }
                        else
                        {
                            (tour == j->j1) ? (tour = j->j2) : (tour = j->j1);
                            (evt_itf->joueur == 0) ? (evt_itf->joueur = 1)
                                                   : (evt_itf->joueur = 0);
                        }
                    }
                    if (!checkVictoire(j->b) && boardComplet(j->b) && stop)
                    {
                        isRunning = false;
                        stop = 0;
                    }
                }
                else if (evt_itf->fenetre->mode_jeu == IAvsIA && !stop && !isRunning)
                {
                    if (checkVictoire(j->b))
                    {
                        if (2 == SDL_Event_Interface_donneZoneClic(evt_itf, x, y))
                        {
                            if (!SDL_Interface_dessinerResultat(evt_itf->fenetre, evt_itf->fenetre->coordonnes->grille))
                                state = false;
                        }
                        else if (2 != SDL_Event_Interface_donneZoneClic(evt_itf, x, y))
                        {
                            if (!SDL_Interface_dessinerGrille(&evt_itf->fenetre, evt_itf->fenetre->coordonnes->grille))
                                state = false;
                        }
                    }
                }
                break;
            }
        }
    }
}

static int SDL_Event_Interface_event_fenetre(SDL_Event_Interface *evt_itf)
{
    SDL_Interface_Param_Window choice = SDL_INTERFACE_WINDOW_SHOW;
    SDL_Interface *itf = evt_itf->fenetre;
    SDL_Event *evt = evt_itf->event;
    switch (evt->window.event)
    {
    case SDL_WINDOWEVENT_CLOSE:
        // Fermeture de la fenêtre
        return -1;
        break;
    case SDL_WINDOWEVENT_SIZE_CHANGED:
    case SDL_WINDOWEVENT_RESIZED:
        // Redimensionnage de la fenêtre
        choice = SDL_INTERFACE_WINDOW_SHOW;
        break;
    case SDL_WINDOWEVENT_RESTORED:
        // Remise des paramètres de la fenêtre à zéro
        choice = SDL_INTERFACE_WINDOW_RESTORE;
        break;
    case SDL_WINDOWEVENT_MAXIMIZED:
        choice = SDL_INTERFACE_WINDOW_MAX;
        // Agrandisement de la fenêtre
        break;
    case SDL_WINDOWEVENT_MINIMIZED:
        choice = SDL_INTERFACE_WINDOW_MIN;
        // Retrecissement de la fenêtre
        break;
    default:
        return 0;
    }
    if (false == SDL_Interface_actualiserTexture(itf, choice, evt_itf->choice))
        return 2;
    else
        return 0;
}

static int SDL_Event_Interface_event_clavier(SDL_Event_Interface *evt_itf)
{
    SDL_Interface *itf = evt_itf->fenetre;
    SDL_Event *evt = evt_itf->event;
    SDL_Keysym touch;
    touch = evt->key.keysym;
    switch (touch.sym)
    {
    case SDLK_r: // Permet de relancer avec le mode de l'ancienne partie ( Touche R for restart
        SDL_Event_Interface_setInfoJeu(itf->jeu->mode, itf->jeu->j1->ia, itf->jeu->j2->ia, evt_itf, 0);
        if (false == SDL_Event_Interface_recreerJeu(evt_itf))
            return 2;
        goto grille;
    case SDLK_s: // Permet de relancer avec la partie avec la dernière option choisie ( Touche S for start)
        evt_itf->choice = 0;
        if (false == SDL_Event_Interface_recreerJeu(evt_itf))
            return 2;
        goto grille;
    case SDLK_q:
        return -1;
    case SDLK_1: // Permet de lancer le jeu avec un joueur contre un joueur par défault
        SDL_Event_Interface_setInfoJeu(JvsJ, USER, USER, evt_itf, 0);
        goto button;
    case SDLK_2: // Permet de lancer le jeu avec un joueur contre une ia de difficulté normale par défault
        SDL_Event_Interface_setInfoJeu(JvsIA, USER, NORMAL, evt_itf, 0);
        goto button;
    case SDLK_3: // Permet de lancer le jeu avec une ia contre une ia toutes de difficultés normale par défault
        SDL_Event_Interface_setInfoJeu(IAvsIA, NORMAL, NORMAL, evt_itf, 0);
        goto button;
    default:
        return 0;
    }
button:
    if (!SDL_Interface_actualiserCoordonees(itf) || !SDL_Interface_dessinerBouttons(&itf, itf->coordonnes->boutons, evt_itf->choice))
        return 2;
    return 0;
grille:
    if (!SDL_Interface_actualiserCoup(itf) || !SDL_Interface_dessinerBouttons(&itf, itf->coordonnes->boutons, evt_itf->choice))
        return 2;
    return 0;
}

static int SDL_Event_Interface_event_souris(SDL_Event_Interface *evt_itf)
{
    SDL_Event *evt = evt_itf->event;
    SDL_Interface *itf = evt_itf->fenetre;
    Uint32 x = evt->button.x, y = evt->button.y;
    int zone = SDL_Event_Interface_donneZoneClic(evt_itf, x, y);
    switch (zone)
    {
    case 0:
        SDL_Event_Interface_checkZoneBoutons(evt_itf, x);
        if (!SDL_Interface_actualiserCoordonees(itf) || !SDL_Interface_dessinerBouttons(&itf, itf->coordonnes->boutons, evt_itf->choice) || !SDL_Interface_dessinerInfo(&itf, itf->coordonnes->info))
            return 2;
        ;
        return 0;
    case 1:
        if (itf->jeu->mode != IAvsIA)
        {
            SDL_Event_Interface_checkZoneGrille(evt_itf, x);
            if (!SDL_Interface_actualiserCoup(itf) && !SDL_Interface_dessinerBouttons(&itf, itf->coordonnes->boutons, 0))
                return 2;
        }
        return 0;
    case 2:
        int res = SDL_Event_Interface_checkZoneInfo(evt_itf, y);
        if (res == 0)
        {
            if (!SDL_Interface_dessinerGrille(&itf, itf->coordonnes->grille))
                return 2;
        }
        else if (1 == res && res == 2)
        {
            if (!SDL_Interface_actualiserCoup(itf) || !SDL_Interface_dessinerBouttons(&itf, itf->coordonnes->boutons, evt_itf->choice))
                return 2;
        }
        else if (-1 == res)
        {
            return -1;
        }
        return 0;
    default:
        return 0;
    }
    return -1;
}

extern int SDL_Event_Interface_donneZoneClic(SDL_Event_Interface *evt_itf, Uint32 x, Uint32 y)
{
    SDL_Interface *itf = evt_itf->fenetre;
    SDL_Interface_Coord *coord = itf->coordonnes;
    SDL_Rect *rect;
    for (int i = 0; i < coord->nbPos; i++)
    {
        switch (i)
        {
        case 0:
            rect = coord->boutons;
            if (x > rect->x && y > rect->y && x < rect->x + rect->w && y < rect->y + rect->h)
            {
                return 0;
            }
            break;
        case 1:
            rect = coord->grille;
            if (x > rect->x && y > rect->y && x < rect->x + rect->w && y < rect->y + rect->h)
            {
                return 1;
            }
            break;
        case 2:
            rect = coord->info;
            if (x > rect->x && y > rect->y && x < rect->x + rect->w && y < rect->y + rect->h)
            {
                return 2;
            }
            break;
        default:
            break;
        }
    }
    return -1;
}

static void SDL_Event_Interface_checkZoneGrille(SDL_Event_Interface *evt_itf, Uint32 x)
{
    SDL_Rect *des = evt_itf->fenetre->coordonnes->grille;
    int new_x;
    for (int i = 0; i < SDL_INTERFACE_NB_COLONNE; i++)
    {
        new_x = (i * (des->w / SDL_INTERFACE_NB_COLONNE)) + des->x;
        if (x > new_x && x < (des->w / SDL_INTERFACE_NB_COLONNE) + new_x)
            SDL_Event_Interface_jouerCoup(evt_itf, i);
    }
}

static void SDL_Event_Interface_checkZoneBoutons(SDL_Event_Interface *evt_itf, Uint32 x)
{
    SDL_Rect *rect = evt_itf->fenetre->coordonnes->boutons;
    int currentX = rect->x;
    for (int i = 0; i < 3; i++)
    {
        currentX = rect->x + i * (rect->w / 3);
        if (x > currentX && x < currentX + (rect->w / 3))
        {
            if (evt_itf->choice == 0)
            {
                switch (i)
                {
                case 0:
                    SDL_Event_Interface_setInfoJeu(JvsJ, USER, USER, evt_itf, 0);
                    break;
                case 1:
                    SDL_Event_Interface_setInfoJeu(JvsIA, USER, NORMAL, evt_itf, 1);
                    break;
                case 2:
                    SDL_Event_Interface_setInfoJeu(IAvsIA, NORMAL, NORMAL, evt_itf, 2);
                    break;
                }
            }
            else if (evt_itf->choice == 1)
            {
                switch (i)
                {
                case 0:
                    evt_itf->fenetre->ia2 = FACILE;
                    break;
                case 1:
                    evt_itf->fenetre->ia2 = NORMAL;
                    break;
                case 2:
                    evt_itf->fenetre->ia2 = DIFFICILE;
                    break;
                }
                evt_itf->choice = 0;
            }
            else
            {
                switch (i)
                {
                case 0:
                    evt_itf->fenetre->ia1 = FACILE;
                    break;
                case 1:
                    evt_itf->fenetre->ia1 = NORMAL;
                    break;
                case 2:
                    evt_itf->fenetre->ia1 = DIFFICILE;
                    break;
                }
                evt_itf->choice = 1;
            }
        }
    }
}

static int SDL_Event_Interface_checkZoneInfo(SDL_Event_Interface *evt_itf, Uint32 y)
{
    SDL_Interface *itf = evt_itf->fenetre;
    SDL_Rect *rect = itf->coordonnes->info;
    int currentY;
    for (int i = 0; i < 3; i++)
    {
        currentY = rect->y + (i * (rect->h / 3));
        if (y > currentY && y < currentY + (rect->h / 3))
        {
            switch (i)
            {
            case 0:
                if (false == SDL_Interface_dessinerGrille(&itf, itf->coordonnes->grille))
                    return -1;
                return 0;
            case 1:
                SDL_Event_Interface_setInfoJeu(itf->mode_jeu, itf->ia1, itf->ia2, evt_itf, 0);
                if (false == SDL_Event_Interface_recreerJeu(evt_itf))
                    return -1;
                return 1;
            case 2:
                SDL_Event_Interface_setInfoJeu(itf->jeu->mode, itf->jeu->j1->ia, itf->jeu->j2->ia, evt_itf, 0);
                if (false == SDL_Event_Interface_recreerJeu(evt_itf))
                    return -1;
                return 2;
            }
        }
    }
    return 0;
}

extern void lancementMVC()
{
    SDL_Interface *interface;
    SDL_Event_Interface *evt_itf;
    char *name = (char *)malloc(sizeof(char) * 4);
    name[0] = 'S';
    name[1] = 'D';
    name[2] = 'L';
    name[3] = '\0';
    interface = startSDL(name, 1000, 1000);
    if (interface != NULL)
    {
        evt_itf = SDL_Event_Interface_creer(interface);
        if (false == SDL_Interface_actualiserTexture(interface, SDL_INTERFACE_WINDOW_SHOW, 0))
            SDL_Event_Interface_detruire(evt_itf);
        else if (evt_itf && getErreur() == ERR_NO_ERROR)
        {
            SDL_Event_Interface_attendre(evt_itf);
            SDL_Event_Interface_detruire(evt_itf);
        }
    }
    free(name);
}

static int SDL_Event_Interface_jouerCoup(SDL_Event_Interface *evt_itf, int i)
{
    Jeu *j = evt_itf->fenetre->jeu;
    Board **board = &(j->b);
    {
        if (!finJeu(j->b))
        {
            if (checkVictoire(j->b))
            {
                return 1;
            }
            if (evt_itf->fenetre->mode_jeu == JvsJ)
            {
                jouerJeton(board, i);
                swapTour(board);
            }
            else if (evt_itf->fenetre->mode_jeu == JvsIA)
            {
                if (evt_itf->joueur == 0 && stop == 0)
                {
                    stop = 1;
                    jouerJeton(board, i);
                    if ((false == SDL_Interface_actualiserCoup(evt_itf->fenetre)))
                        return -1;
                    sleep(1);
                    if (checkVictoire(j->b))
                    {
                        return 1;
                    }
                    swapTour(board);
                    j->j2->play(j);
                    if (checkVictoire(j->b))
                    {
                        return 1;
                    }
                    stop = 0;
                }
                return 0;
            }
        }
        else
        {
            if (!checkVictoire(j->b) && boardComplet(j->b))
            {
                return 2;
            }
            return 0;
        }
        return -1;
    }
}

extern bool SDL_Event_Interface_recreerJeu(SDL_Event_Interface *evt_itf)
{
    SDL_Interface *itf = evt_itf->fenetre;
    freeJeu(itf->jeu);
    if (false == SDL_Interface_creerJeu(itf))
        return false;
    isRunning = false;
    evt_itf->choice = 0;
    if (itf->mode_jeu == JvsJ)
    {
        itf->ia1 = USER;
        itf->ia2 = USER;
    }
    if (itf->mode_jeu == JvsIA)
    {
        itf->ia1 = USER;
        if (evt_itf->joueur == 1)
        {
            stop = 1;
            itf->jeu->j2->play(itf->jeu);
            if ((false == SDL_Interface_actualiserCoup(evt_itf->fenetre)))
                return false;
            sleep(1);
        }
        evt_itf->joueur = 0;
        stop = 0;
    }
    else if (itf->mode_jeu == IAvsIA)
    {
        stop = 1;
        isRunning = true;
    }
    printf("Mode : %d , ia1 : %d , ia2 : %d\n", itf->mode_jeu, itf->ia1, itf->ia2);
    return true;
}

extern void SDL_Event_Interface_setInfoJeu(ModeDeJeu mdj, IA ia1, IA ia2, SDL_Event_Interface *evt_itf, int choice)
{
    SDL_Interface_modifierJeu(evt_itf->fenetre, mdj, ia1, ia2);
    evt_itf->joueur = 0;
    int r = random() % 2;
    if (r == 0)
    {
        evt_itf->joueur = 1;
    }
    evt_itf->choice = choice;
    if (mdj == IAvsIA)
        stop = 1;
    else
        stop = 0;
}
