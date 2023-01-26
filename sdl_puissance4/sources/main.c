#include "../headers/main.h"

int main(int argc, char** argv){
    int state = EXIT_SUCCESS;

    /// Vérification des arguments
    if (argc < 2){
        setErreur(ERR_INVALID_OPTION);
        fprintf(stderr,"[ARGS] Argument manquant . Choix par défaut de la sortie : Console\n");
    }else if ( argc == 2){
        
        /////// Pour la sortie standard
        if (!strcmp(argv[1], "-c")){
            jouerTexte();

            // Board* b = creerBoard();
            // int tab[6][7] = {
            //     {0,0,0,0,0,0,0},
            //     {0,0,0,0,0,0,0},
            //     {0,0,0,0,0,2,0},
            //     {0,0,0,1,2,2,2},
            //     {0,0,1,1,2,1,1},
            //     {1,2,2,2,1,1,2},
            // };
            // DEBUG_jetonsManuel(&b, tab);
            // coupGagnant(b);


        /////// Pou la sortie sur SDL
        }else if (!strcmp(argv[1],"-s")){
            lancementMVC();
        ////////// Si il n'y a pas de sortie choisie.
        }else{
            setErreur(ERR_INVALID_OPTION);
            fprintf(stderr,"[ARGS] Argument incorrect . Type de paramètre inconnu : Fin du programme\n");
        }
        ////////// Si il n'y a pas le nombre d'arguments correct choisie.
    }else{
        fprintf(stderr,"[ARGS] Arguments invalides . Trop de paramètres : Fin du programme\n");
        state = EXIT_FAILURE;
        setErreur(ERR_INVALID_OPTION);
    }
    ////// Gestion des erreurs
    if(state == EXIT_SUCCESS)
        return state;
    else
        return getErreur();

}
