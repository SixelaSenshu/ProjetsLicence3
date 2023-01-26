#include "../../headers/Jeu/IA.h"
#include <time.h>
#include <stdlib.h>

void jouer_FACILE (Jeu* jeu) {
    bool ok;
    do {
        srand(time(NULL));
        int r = rand() % 7;
        ok = jouerJeton(&(jeu->b), r);
    } while (!ok);
    swapTour(&jeu->b);
}

bool jouerCoupGagnant (Jeu *jeu) {
    int* cg = coupGagnant(jeu->b);
    if (cg[1] == -1 && cg[3] == -1) {
        free(cg);
        return false;
    }
    int joueur = jeu->b->tour;
    if (joueur == 1) {
        if (cg[1] > -1) jouerJeton(&(jeu->b), cg[1]);
        else jouerJeton(&(jeu->b), cg[3]);
    } else {
        if (cg[3] > -1) jouerJeton(&(jeu->b), cg[3]);
        else jouerJeton(&(jeu->b), cg[1]);
    }
    free(cg);
    swapTour(&jeu->b);
    return true;
}

void jouer_NORMAL (Jeu* jeu) {
    if (jouerCoupGagnant(jeu)) return;
    bool ok;
    do {
        srand(time(NULL));
        int r = rand() % 7;
        ok = jouerJeton(&(jeu->b), r);
    } while (!ok);
    swapTour(&jeu->b);
}

void jouer_DIFFICILE (Jeu* jeu) {
    if (jeu->b->dernierJeton[0] == -1) { //si l'ia commence, joue au centre, c'est le meilleur choix
        jouerJeton(&jeu->b, 3);
        swapTour(&jeu->b);
        return;
    }
    if (jouerCoupGagnant(jeu)) { //si elle peut gagner ou bloque un coup gagnant adverse
        return;
    }
    int* grade = (int*) malloc(sizeof(int)*7);
    if (!grade) {
        setErreur(ERR_ERROR_ALLOCATION);
        return;
    }
    predictionCoups(&grade, jeu); //remplissage de grade avec cette fonction
    int max = -1, nb = 0; //cherche le max dans grade et le nombre de colonne avec le même score
    for (int i = 0; i < 7; i += 1) {
        if (grade[i] == 5) { //si le grade est à 5, ignore tout et joue
            jouerJeton(&jeu->b, i);
            free(grade);
            swapTour(&jeu->b);
            return;
        }
        if (grade[i] && grade[i] > max) {
            max = grade[i];
            nb = 1;
        } else if (grade[i] == max) {
            nb += 1;
        }
    }
    srand(time(NULL));
    int r = rand() % nb;
    int ind;
    for (int i = 0; i < 7; i += 1) { //choisit une colonne random parmi les max
        if (grade[i] != max) continue;
        if (r == 0) {
            ind = i;
            break;
        }
        r -= 1;
    }
    jouerJeton(&jeu->b, ind);
    swapTour(&jeu->b);
    free(grade);
}

void jouer_USER (Jeu* jeu) {
    bool ok;
    do {
        printf("Entrez la colonne dans laquelle vous voulez jouer (entre 0 et 6)\n");
        char* input = (char*) malloc(sizeof(char)*5);
        if (!input) {
            setErreur(ERR_ERROR_ALLOCATION);
            return;
        }
        fgets(input, 4, stdin);
        ok = jouerJeton(&(jeu->b), atoi(input));
        free(input);
        if (!ok) {
            printf("Mauvaise entrée, recommencez.\n");
        }
    } while (!ok);
    swapTour(&jeu->b);
}

int* coupGagnant (Board* b) {
    int* gagne = (int*) malloc(sizeof(int)*4);
    if (!gagne) {
        setErreur(ERR_ERROR_ALLOCATION);
        return NULL;
    }
    for (int i = 0; i < 4; i += 1) {
        gagne[i] = -1;
    }
    coupGagnantColonne(b, &gagne);
    coupGagnantLigne(b, &gagne);
    coupGagnantDiagonale(b, &gagne);
    return gagne;
}

void coupGagnantColonne (Board* b, int** gagne) {
    int num; //retient le jeton recherché, 1 pour j1, 2 pour j2
    int nb; //nb de jeton $num trouvés d'affilée
    int sommet; //plus haut jeton d'une suite de jetons identique
    for (int i = 0; i < 7; i += 1) {
        num = 0;
        nb = 0;
        for (int j = 0; j < 6; j += 1) {
            if (!b->jetons[j][i]) continue;
            if (!num) {
                num = b->jetons[j][i];
                nb = 1;
                sommet = j;
                continue;
            }
            if (b->jetons[j][i] != num) break;
            if (b->jetons[j][i] == num) nb += 1;
            if (nb == 3) {
                if (sommet > 0) {  //si on est pas tout en haut, alors coup gagnant
                    if (num == 1) {
                        (*gagne)[0] = sommet - 1;
                        (*gagne)[1] = i;
                    } else {
                        (*gagne)[2] = sommet - 1;
                        (*gagne)[3] = i;
                    }
                }
                break;
            }
        }
    }
}

void coupGagnantLigne (Board* b, int** gagne) {
    int num; //retient le jeton recherché, 1 pour j1, 2 pour j2
    int nb; //nb de jeton $num trouvés d'affilée
    int indPremier; //indice du premier jeton rencontré de la couleur num
    for (int i = 5; i >= 0; i -= 1) { // check ligne par ligne en commençant par la dernière
        num = 0;
        nb = 0;
        indPremier = -1;
        for (int j = 0; j < 7; j += 1) {
            if (!b->jetons[i][j]) { //si case vide, réinitialise num, nb et indPremier et go next
                num = 0;
                nb = 0;
                indPremier = -1;
            } else if (!num) { //on trouve une case non vide, si num est toujours a 0 on stoque la couleur dans num, nb est a 1 et indPremier à j
                num = b->jetons[i][j];
                nb = 1;
                indPremier = j;
            } else if (b->jetons[i][j] != num) { //num différent donc changement de num, nb a 1 et indPremier a j de nouveau
                num = b->jetons[i][j];
                nb = 1;
                indPremier = j;
            } else if (b->jetons[i][j] == num) nb += 1; //meme num donc on incrémente nb
            if (nb == 3) { //si on trouve 3 jetons de la meme couleur d'affilée
                if (indPremier < 4) { //si ils ne sont pas tout à droite
                    //si la case après ces 3 jetons est vide && si c'est la dernière ligne ou bien si il y a un jeton sous la case a droite
                    if (!b->jetons[i][indPremier+3] && (i == 5 || b->jetons[i+1][indPremier+3])) { 
                        if (num == 1) {
                            (*gagne)[0] = i;
                            (*gagne)[1] = indPremier+3;
                        } else {
                            (*gagne)[2] = i;
                            (*gagne)[3] = indPremier+3;
                        }
                    }
                }
                if (indPremier > 0) { //si ils ne sont pas tout à gauche
                    //si la case avant ces 3 jetons est vide && si c'est la dernière ligne ou bien si il y a un jeton sous la case à gauche
                    if (!b->jetons[i][indPremier-1] && (i == 5 || b->jetons[i+1][indPremier-1])) {
                        if (num == 1) {
                            (*gagne)[0] = i;
                            (*gagne)[1] = indPremier-1;
                        } else {
                            (*gagne)[2] = i;
                            (*gagne)[3] = indPremier-1;
                        }
                    }
                }
            }
        } //fin for j
    } //fin for i
}

void coupGagnantDiagonaleHautGauche (Board* b, int x, int y, int** gagne){
    if (x <= 2 || y <= 2) return;
    int p1 = b->jetons[x--][y--];
    int p2 = b->jetons[x--][y--];
    int p3 = b->jetons[x--][y--];
    if ((p1 == p2) && (p1 == p3) && (p1) && b->jetons[x+1][y] && (!b->jetons[x][y])) {
        if (p1 == 1) {
            (*gagne)[0] = x;
            (*gagne)[1] = y;
        } else if (p1 == 2) {
            (*gagne)[2] = x;
            (*gagne)[3] = y;
        }
    }
}

void coupGagnantDiagonaleHautDroite (Board* b, int x, int y, int** gagne){
    if (x <= 2 || y >= 4) return;
    int p1 = b->jetons[x--][y++];
    int p2 = b->jetons[x--][y++];
    int p3 = b->jetons[x--][y++];
    if ((p1 == p2) && (p1 == p3) && (p1) && b->jetons[x+1][y] && (!b->jetons[x][y])) {
        if (p1 == 1) {
            (*gagne)[0] = x;
            (*gagne)[1] = y;
        } else if (p1 == 2) {
            (*gagne)[2] = x;
            (*gagne)[3] = y;
        }
    }
}

void coupGagnantDiagonaleBasGauche (Board* b, int x, int y, int** gagne){
    if (x >= 3 || y <= 2) return;
    int p1 = b->jetons[x++][y--];
    int p2 = b->jetons[x++][y--];
    int p3 = b->jetons[x++][y--];
    if ((p1 == p2) && (p1 == p3) && (p1) && (x == 5 || b->jetons[x+1][y]) && (!b->jetons[x][y])) {
        if (p1 == 1) {
            (*gagne)[0] = x;
            (*gagne)[1] = y;
        } else if (p1 == 2) {
            (*gagne)[2] = x;
            (*gagne)[3] = y;
        }
    }
}

void coupGagnantDiagonaleBasDroite (Board* b, int x, int y, int** gagne) {
    if (x >= 3 || y >= 4) return;
    int p1 = b->jetons[x++][y++];
    int p2 = b->jetons[x++][y++];
    int p3 = b->jetons[x++][y++];
    if ((p1 == p2) && (p1 == p3) && (p1) && (x == 5 || b->jetons[x+1][y]) && (!b->jetons[x][y])) {
        if (p1 == 1) {
            (*gagne)[0] = x;
            (*gagne)[1] = y;
        } else if (p1 == 2) {
            (*gagne)[2] = x;
            (*gagne)[3] = y;
        }
    }
}

void coupGagnantMilieuDiagoHGBD (Board *b, int x, int y, int** gagne) {
    if (x == 0 || x == 5 || y == 0 || y == 6 || b->jetons[x][y]) return; //cas gérés par les autres fonctions pour les diagonales
    int j;
    int nb = 0;
    if (b->jetons[x-1][y-1]) {
        j = b->jetons[x-1][y-1];
        nb += 1;
        if ((x-2 >= 0) && (y-2 >= 0)) {
            if (b->jetons[x-2][y-2] == j) nb += 1;
        }
    } else return;
    if (b->jetons[x+1][y+1] == j) {
        nb += 1;
        if ((x+2 <= 5) && (y+2 <= 6)) {
            if (b->jetons[x+2][y+2] == j) nb += 1;
        }
    }
    if (nb >= 3) {
        if (b->jetons[x+1][y]) {
            if (j == 1) {
                (*gagne)[0] = x;
                (*gagne)[1] = y;
            } else if (j == 2) {
                (*gagne)[2] = x;
                (*gagne)[3] = y;
            }
        }
    }
}

void coupGagnantMilieuDiagoHDBG (Board *b, int x, int y, int** gagne) {
    if (x == 0 || x == 5 || y == 0 || y == 6 || b->jetons[x][y]) return; //cas gérés par les autres fonctions pour les diagonales
    int j;
    int nb = 0;
    if (b->jetons[x-1][y+1]) {
        j = b->jetons[x-1][y+1];
        nb += 1;
        if ((x-2 >= 0) && (y+2 <= 6)) {
            if (b->jetons[x-2][y+2] == j) nb += 1;
        }
    } else return;
    if (b->jetons[x+1][y-1] == j) {
        nb += 1;
        if ((x+2 <= 5) && (y-2 >= 0)) {
            if (b->jetons[x+2][y-2] == j) nb += 1;
        }
    }
    if (nb >= 3) {
        if (b->jetons[x+1][y]) {
            if (j == 1) {
                (*gagne)[0] = x;
                (*gagne)[1] = y;
            } else if (j == 2) {
                (*gagne)[2] = x;
                (*gagne)[3] = y;
            }
        }
    }
}

void coupGagnantMilieuLigne (Board *b, int x, int y, int **gagne) {
    if (b->jetons[x][y]) return;
    if (y == 0 || y == 6) return;
    if (x != 5) if (!b->jetons[x+1][y]) return;
    int j; //couleur regardée
    int nb = 0;
    if (!b->jetons[x][y-1]) return;
    if (b->jetons[x][y-1]) {
        j = b->jetons[x][y-1];
        nb += 1;
        if (y != 1) {
            if (b->jetons[x][y-2] == j) nb += 1;
        }
    }
    if (b->jetons[x][y+1] != j) return;
    else {
        nb += 1;
        if (y != 5) {
            if (b->jetons[x][y+2] == j) nb += 1;
        }
    }
    if (nb >= 3) {
        if (j == 1) {
            (*gagne)[0] = x;
            (*gagne)[1] = y;
        } else if (j == 2) {
            (*gagne)[2] = x;
            (*gagne)[3] = y;
        }
    }
}

void coupGagnantDiagonale (Board* b, int** gagne) {
    for (int i = 0; i < 6; i += 1) { //lignes
        for (int j = 0; j < 7; j += 1) { //colonnes
            coupGagnantDiagonaleHautGauche(b, i, j, gagne);
            coupGagnantDiagonaleHautDroite(b, i, j, gagne);
            coupGagnantDiagonaleBasGauche(b, i, j, gagne);
            coupGagnantDiagonaleBasDroite(b, i, j, gagne);
            coupGagnantMilieuDiagoHGBD(b, i, j, gagne);
            coupGagnantMilieuDiagoHDBG(b, i, j, gagne);
            coupGagnantMilieuLigne(b, i, j, gagne);
        }
    }
}

bool coupGagnantJoueur (Jeu *jeu, int joueur) {
    int *cg = coupGagnant(jeu->b);
    if (joueur == 1) {
        if (cg[0] == -1) goto faux;
        else goto vrai;
    } else {
        if (cg[2] == -1) goto faux;
        else goto vrai;
    }
    vrai:
    free(cg);
    return true;
    faux:
    free(cg);
    return false;
}