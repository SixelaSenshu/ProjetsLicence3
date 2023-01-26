#include "../../headers/Jeu/Board.h"
#include <stdlib.h>
#include <stdio.h>

Board* creerBoard () {
    Board *b = (Board*) malloc(sizeof(Board));
    if (!b) {
        setErreur(ERR_ERROR_ALLOCATION);
        return NULL;
    }
    b->tour = 1;
    for (int i = 0; i < 6; i += 1) {
        for (int j = 0; j < 7; j += 1) {
            b->jetons[i][j] = 0;
        }
    }
    b->dernierJeton[0] = -1;
    b->dernierJeton[1] = -1;
    return b;
}

void freeBoard (Board *b) {
    if (b) free(b);
}

bool jouerJeton (Board **b, int col) {
    if (!*b) {
        setErreur(ERR_INVALID_POINTER);
        return false;
    }
    if (col < 0 || col > 6) {
        return false;
    }
    if ((*b)->jetons[0][col]) {
        return false;
    }
    int plusBasJeton = 6;
    for (int i = 5; i >= 0; i -= 1) {
        if ((*b)->jetons[i][col]) {
            plusBasJeton -= 1;
        } else {
            break;
        }
    }
    (*b)->jetons[plusBasJeton-1][col] = (*b)->tour;
    (*b)->dernierJeton[0] = plusBasJeton-1;
    (*b)->dernierJeton[1] = col;
    return true;
}

void afficherBoard (Board *b) {
    if (!b) {
        setErreur(ERR_INVALID_POINTER);
        return;
    }
    for (int i = 0; i < 6; i += 1) {
        printf("+-+-+-+-+-+-+-+\n");
        for (int j = 0; j < 7; j += 1) {
            if (!b->jetons[i][j]) printf("|-");
            else printf("|%d", b->jetons[i][j]);
        }
        printf("|\n");
    }
    printf("+-+-+-+-+-+-+-+\n");
}

void swapTour (Board **b) {
    ((*b)->tour == 1) ? ((*b)->tour = 2) : ((*b)->tour = 1);
}

bool checkVictoireDiagonales (Board *b) {
    int x = b->dernierJeton[0];
    int y = b->dernierJeton[1];
    int joueur = b->jetons[x][y]; //quel joueur a posé le dernier jeton
    int num_hgbd = 1; //num jetons identiques diagonale haut gauche bas droite
    for (int i = x-1, j = y-1; i >= 0 && j >= 0; i -= 1, j -= 1) { //haut gauche
        if (b->jetons[i][j] != joueur) break;
        num_hgbd += 1;
    }
    for (int i = x+1, j = y+1; i <= 5 && j <= 6; i += 1, j += 1) { //bas droite
        if (b->jetons[i][j] != joueur) break;
        num_hgbd += 1;
    }
    if (num_hgbd >= 4) return true;
    int num_hdbg = 1; //num jetons identiques diagonale haut droite bas gauche
    for (int i = x-1, j = y+1; i >= 0 && j <= 6; i -= 1, j += 1) { //haut droite
        if (b->jetons[i][j] != joueur) break;
        num_hdbg += 1;
    }
    for (int i = x+1, j = y-1; i <= 5 && j >= 0; i += 1, j -= 1) { //bas gauche
        if (b->jetons[i][j] != joueur) break;
        num_hdbg += 1;
    }
    if (num_hdbg >= 4) return true;
    return false;
}

bool checkVictoire (Board *b) {
    int x = b->dernierJeton[0];
    int y = b->dernierJeton[1];
    if (x == -1 || y == -1) {
        return false;
    }
    int j = b->jetons[x][y]; //quel joueur a posé le dernier jeton
    if (x <= 2) { //check si victoire sur la colonne
        int num = 1;
        for (int i = x+1; i < x+4; i += 1) {
            if (b->jetons[i][y] == j) {
                num += 1;
            } else {
                break;
            }
        }
        if (num == 4) return true;
    }
    int num = 1; //check ligne à partir d'ici
    int tmp; //variable qui stoque les jetons regardés
    int cursor = y - 1; //colonne où l'on regarde: d'abord vers la gauche puis vers la droite
    while (1) { //check gauche
        if (cursor == -1) {
            break;
        }
        tmp = b->jetons[x][cursor];
        if (tmp == j) num += 1;
        else break;
        cursor -= 1;
    }
    cursor = y + 1;
    while (1) { //check droite
        if (cursor == 7) {
            break;
        }
        tmp = b->jetons[x][cursor];
        if (tmp == j) num += 1;
        else break;
        cursor += 1;
    }
    if (num >= 4) return true;
    return checkVictoireDiagonales(b);
}

bool boardComplet (Board *b) {
    for (int i = 0; i < 6; i += 1) {
        for (int j = 0; j < 7; j += 1) {
            if (!b->jetons[i][j]) return false;
        }
    }
    return true;
}

bool finJeu (Board *b) {
    return (checkVictoire(b) || boardComplet(b));
}

void DEBUG_jetonsManuel (Board **b, int tab[6][7]) {
    for (int i = 0; i < 6; i += 1) {
        for (int j = 0; j < 7; j += 1) {
            (*b)->jetons[i][j] = tab[i][j];
        }
    }
}

Board* copyBoard (Board *b) {
    if (!b) return NULL;
    Board *new = creerBoard();
    if (!new) return NULL;
    new->tour = b->tour;
    new->dernierJeton[0] = b->dernierJeton[0];
    new->dernierJeton[1] = b->dernierJeton[1];
    for (int i = 0; i < 6; i += 1) {
        for (int j = 0; j < 7; j += 1) {
            new->jetons[i][j] = b->jetons[i][j];
        }
    }
    return new;
}

void annuleCoup (Board **b) {
    if (!*b) return;
    if ((*b)->dernierJeton[0] == -1) return;
    (*b)->jetons[(*b)->dernierJeton[0]][(*b)->dernierJeton[1]] = 0;
}