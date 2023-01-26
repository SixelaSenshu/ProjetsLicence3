#include "../../headers/Jeu/InfoIA.h"
#include "../../headers/macros.h"

Section* creerSection (TypeLigne t, int couleur, int nbJetons, int x, int y, ...) {
    va_list args;
    Section* s = (Section*) malloc(sizeof(Section));
    if (!s) {
        setErreur(ERR_ERROR_ALLOCATION);
        return NULL;
    }
    s->jetons = (int*) malloc(sizeof(int)*nbJetons*2);
    if (!s->jetons) {
        setErreur(ERR_ERROR_ALLOCATION);
        return NULL;
    }
    s->jetons[0] = x;
    s->jetons[1] = y;
    s->nbJetons = nbJetons;
    s->type = t;
    s->couleur = couleur;

    va_start(args, y);
    for (int i = 0; i < nbJetons-1; i += 1) {
        s->jetons[i*2+2] = va_arg(args, int);
        s->jetons[i*2+3] = va_arg(args, int);
    }
    return s;
}

TableauSections* creerTableauSections () {
    TableauSections* ts = (TableauSections*) malloc(sizeof(TableauSections));
    if (!ts) {
        setErreur(ERR_ERROR_ALLOCATION);
        return NULL;
    }
    ts->nbSections = 0;
    ts->s = NULL;
    return ts;
}

void ajouteSectionTS (Section* section, TableauSections **ts) {
    if (!section) return;
    if (!(*ts)->s) {
        (*ts)->s = (Section**) malloc(sizeof(Section*)*(++(*ts)->nbSections));
    } else {
        (*ts)->s = (Section**) realloc((*ts)->s, sizeof(Section*)*(++(*ts)->nbSections));
    }
    if (!(*ts)->s) {
        setErreur(ERR_ERROR_ALLOCATION);
        return;
    }
    (*ts)->s[(*ts)->nbSections-1] = section;
}

void chercherSectionsColonnes (Board *b, TableauSections **ts) {
    int couleur, nb;
    for (int i = 0; i < 7; i += 1) {
        couleur = 0;
        nb = 0;
        for (int j = 5; j >= 0; j -= 1) {
            int jeton = b->jetons[j][i];
            if (!jeton && !couleur) break;
            if (!couleur) {
                couleur = jeton;
                nb = 1;
            } else if (couleur == jeton) nb += 1;
            if ((couleur != jeton) || j == 0) {
                if (j == 0) j -= 1;
                Section *s;
                if (nb == 2) s = creerSection(COLONNE, couleur, nb, j+1, i, j+2, i);
                else if (nb == 3) s = creerSection(COLONNE, couleur, nb, j+1, i, j+2, i, j+3, i);
                if (nb >= 2) ajouteSectionTS(s, ts);
                if (j > 0) {
                    nb = 1;
                    couleur = jeton;
                }
            }
        }
    }
}

void chercherSectionsLignes (Board *b, TableauSections **ts) {
    int couleur, nb;
    for (int i = 0; i < 6; i += 1) {
        couleur = 0;
        nb = 0;
        for (int j = 0; j < 7; j += 1) {
            int jeton = b->jetons[i][j];
            if (!jeton && !couleur) continue;
            if (!couleur) {
                couleur = jeton;
                nb = 1;
            } else if (couleur == jeton) nb += 1;
            if ((couleur != jeton) || j == 6) {
                if (j == 6 && couleur == jeton) j += 1;
                Section *s;
                if (nb == 2) s = creerSection(LIGNE, couleur, nb, i, j-1, i, j-2);
                else if (nb == 3) s = creerSection(LIGNE, couleur, nb, i, j-1, i, j-2, i, j-3);
                if (nb >= 2) ajouteSectionTS(s, ts);
                if (j < 6) {
                    nb = 1;
                    couleur = jeton;
                }
            }
        }
    }
}

void HGBD_Gauche (Board *b, TableauSections **ts) {
    int nb, couleur;
    for (int i = 1; i < 6; i += 1) {
        nb = 0;
        couleur = 0;
        for (int j = 0, k = i; j < 7 && k < 6; j += 1, k += 1) {
            int jeton = b->jetons[k][j];
            if (jeton != couleur || k == 5) {
                if (k == 5) {
                    k += 1;
                    j += 1;
                }
                Section *s;
                if (nb == 2) s = creerSection(DIAGONALE, couleur, nb, k-1, j-1, k-2, j-2);
                else if (nb == 3) s = creerSection(DIAGONALE, couleur, nb, k-1, j-1, k-2, j-2, k-3, j-3);
                if (nb >= 2) ajouteSectionTS(s, ts);
                if (k < 5) {
                    nb = 1;
                    couleur = jeton;
                }
                continue;
            }
            if (!jeton) {
                couleur = 0;
                nb = 0;
                continue;
            }
            if (!couleur) {
                couleur = jeton;
                nb = 1;
                continue;
            }
            if (jeton == couleur) nb += 1;
        }
    }
}

void HGBD_Haut (Board *b, TableauSections **ts) {
    int nb, couleur;
    for (int i = 0; i < 7; i += 1) {
        nb = 0;
        couleur = 0;
        for (int j = 0, k = i; j < 6 && k < 7; j += 1, k += 1) {
            int jeton = b->jetons[j][k];
            if (jeton != couleur || k == 6 || j == 5) {
                if ((k == 6 || j == 5) && jeton == couleur) {
                    k += 1;
                    j += 1;
                }
                Section *s;
                if (nb == 2) s = creerSection(DIAGONALE, couleur, nb, j-1, k-1, j-2, k-2);
                else if (nb == 3) s = creerSection(DIAGONALE, couleur, nb, j-1, k-1, j-2, k-2, j-3, k-3);
                if (nb >= 2) ajouteSectionTS(s, ts);
                if (k < 6 && j < 5) {
                    nb = 1;
                    couleur = jeton;
                }
                continue;
            }
            if (!jeton) {
                couleur = 0;
                nb = 0;
                continue;
            }
            if (!couleur) {
                couleur = jeton;
                nb = 1;
                continue;
            }
            if (jeton == couleur) nb += 1;
        }
    }
}

void HDBG_Haut (Board *b, TableauSections **ts) {
    int nb, couleur;
    for (int i = 0; i < 7; i += 1) {
        nb = 0;
        couleur = 0;
        for (int j = 0, k = i; j < 6 && k >= 0; j += 1, k -= 1) {
            int jeton = b->jetons[j][k];
            if (jeton != couleur || k == 0 || j == 5) {
                if ((k == 0 || j == 5) && jeton == couleur) {
                    k -= 1;
                    j += 1;
                }
                Section *s;
                if (nb == 2) s = creerSection(DIAGONALE, couleur, nb, j-1, k+1, j-2, k+2);
                else if (nb == 3) s = creerSection(DIAGONALE, couleur, nb, j-1, k+1, j-2, k+2, j-3, k+3);
                if (nb >= 2) ajouteSectionTS(s, ts);
                if (k > 0 && j < 5) {
                    nb = 1;
                    couleur = jeton;
                }
                continue;
            }
            if (!jeton) {
                couleur = 0;
                nb = 0;
                continue;
            }
            if (!couleur) {
                couleur = jeton;
                nb = 1;
                continue;
            }
            if (jeton == couleur) nb += 1;
        }
    }
}

void HDBG_Droite (Board *b, TableauSections **ts) {
    int nb, couleur;
    for (int i = 1; i < 6; i += 1) {
        nb = 0;
        couleur = 0;
        for (int j = 6, k = i; j >= 0 && k < 6; j -= 1, k += 1) {
            int jeton = b->jetons[k][j];
            if (jeton != couleur || k == 5 || j == 0) {
                if ((k == 5 || j == 0) && jeton == couleur) {
                    k += 1;
                    j -= 1;
                }
                Section *s;
                if (nb == 2) s = creerSection(DIAGONALE, couleur, nb, k-1, j+1, k-2, j+2);
                else if (nb == 3) s = creerSection(DIAGONALE, couleur, nb, k-1, j+1, k-2, j+2, k-3, j+3);
                if (nb >= 2) ajouteSectionTS(s, ts);
                if (k < 6 && j > 0) {
                    nb = 1;
                    couleur = jeton;
                }
                continue;
            }
            if (!jeton) {
                couleur = 0;
                nb = 0;
                continue;
            }
            if (!couleur) {
                couleur = jeton;
                nb = 1;
                continue;
            }
            if (jeton == couleur) nb += 1;
        }
    }
}

void chercherSectionsDiagonales (Board *b, TableauSections **ts) {
    HGBD_Gauche(b, ts);
    HGBD_Haut(b, ts);
    HDBG_Haut(b, ts);
    HDBG_Droite(b, ts);
}

void chercherSectionsRestantes (Board *b, TableauSections **ts) {
    for (int i = 0; i < 6; i += 1) {
        for (int j = 0; j < 7; j += 1) {
            if (!b->jetons[i][j]) continue;
            if (cherchePointTS(i, j, ts)) continue;
            ajouteSectionTS(creerSection(POINT, b->jetons[i][j], 1, i, j), ts);
        }
    }
}

bool cherchePointSection (int x, int y, Section* s) {
    for (int j = 0; j < s->nbJetons; j += 1) {
        if (s->jetons[j*2] == x && s->jetons[j*2+1] == y) return true;
    }
    return false;
}

bool chercheExtremiteSection (int x, int y, Section* s) {
    for (int j = 0; j < s->nbJetons; j += 1) {
        if (s->e1[0] == x && s->e1[1] == y) return true;
        if (s->e2[0] == x && s->e2[1] == y) return true;
    }
    return false;
}

bool cherchePointTS (int x, int y, TableauSections **ts) {
    for (int i = 0; i < (*ts)->nbSections; i += 1) {
        Section *s = (*ts)->s[i];
         if (cherchePointSection(x, y, s)) return true;
    }
    return false;
}

void calculeExtremite (Board *b, Section **s) {
    if ((*s)->type == POINT) {
        (*s)->ext1 = -1;
        (*s)->ext2 = -1;
        (*s)->e1[0] = -1;
        (*s)->e1[1] = -1;
        (*s)->e2[0] = -1;
        (*s)->e2[1] = -1;
    } else if ((*s)->type == COLONNE) { //Extremité 2 non prise en compte (dessous de la colonne donc inchangeable)
        (*s)->ext2 = -1;
        (*s)->e2[0] = -1;
        (*s)->e2[1] = -1;
        (*s)->e1[0] = (*s)->jetons[0]-1;
        (*s)->e1[1] = (*s)->jetons[1];
        if ((*s)->jetons[0] == 0) (*s)->ext1 = -1;
        else (*s)->ext1 = b->jetons[(*s)->e1[0]][(*s)->e1[1]];
    } else if ((*s)->type == LIGNE) {
        (*s)->e2[0] = (*s)->jetons[0];
        (*s)->e2[1] = (*s)->jetons[1]+1;
        (*s)->e1[0] = (*s)->jetons[0];
        (*s)->e1[1] = (*s)->jetons[(*s)->nbJetons*2-1]-1;
        if ((*s)->jetons[1] == 6) (*s)->ext2 = -1;
        else (*s)->ext2 = b->jetons[(*s)->e2[0]][(*s)->e2[1]];
        if ((*s)->jetons[(*s)->nbJetons*2-1] == 0) (*s)->ext1 = -1;
        else (*s)->ext1 = b->jetons[(*s)->e1[0]][(*s)->e1[1]];
    } else if ((*s)->type == DIAGONALE) {
        if (directionSectionDiagonale(*s) == 1) { //besoin de savoir l'inclination de la diagonale en question
            (*s)->e2[0] = (*s)->jetons[0]+1;
            (*s)->e2[1] = (*s)->jetons[1]+1;
            (*s)->e1[0] = (*s)->jetons[(*s)->nbJetons*2-2]-1;
            (*s)->e1[1] = (*s)->jetons[(*s)->nbJetons*2-1]-1;
            if ((*s)->jetons[0] == 5 || (*s)->jetons[1] == 6) (*s)->ext2 = -1;
            else (*s)->ext2 = b->jetons[(*s)->e2[0]][(*s)->e2[1]];
            if ((*s)->jetons[(*s)->nbJetons*2-2] == 0 || (*s)->jetons[(*s)->nbJetons*2-1] == 0) (*s)->ext1 = -1;
            else (*s)->ext1 = b->jetons[(*s)->e1[0]][(*s)->e1[1]];
        } else {
            (*s)->e2[0] = (*s)->jetons[0]+1;
            (*s)->e2[1] = (*s)->jetons[1]-1;
            (*s)->e1[0] = (*s)->jetons[(*s)->nbJetons*2-2]-1;
            (*s)->e1[1] = (*s)->jetons[(*s)->nbJetons*2-1]+1;
            if ((*s)->jetons[0] == 5 || (*s)->jetons[1] == 0) (*s)->ext2 = -1;
            else (*s)->ext2 = b->jetons[(*s)->e2[0]][(*s)->e2[1]];
            if ((*s)->jetons[(*s)->nbJetons*2-2] == 0 || (*s)->jetons[(*s)->nbJetons-1] == 6) (*s)->ext1 = -1;
            else (*s)->ext1 = b->jetons[(*s)->e1[0]][(*s)->e1[1]];
        }
    }
}

void calculeExtremiteTS (Board *b, TableauSections **ts) {
    for (int i = 0; i < (*ts)->nbSections; i += 1) {
        calculeExtremite(b, &((*ts)->s[i]));
    }
}

void chercherSections (Board *b, TableauSections **ts) {
    chercherSectionsColonnes(b, ts);
    chercherSectionsLignes(b, ts);
    chercherSectionsDiagonales(b, ts);
    chercherSectionsRestantes(b, ts);
    calculeExtremiteTS(b, ts);
}

void freeSection (Section* s) {
    if (!s) return;
    if (s->jetons) free(s->jetons);
    free(s);
}

void freeTableauSections (TableauSections* ts) {
    if (!ts) return;
    if (ts->s) {
        for (int i = 0; i < ts->nbSections; i += 1) {
            freeSection(ts->s[i]);
        }
        free(ts->s);
    }
    free(ts);
}

void softFreeTableauSections (TableauSections* ts) {
    if (!ts) return;
    if (ts->s) free(ts->s);
    free(ts);
}

bool coordonneeXestDansSection (int x, Section* s) {
    for (int i = 0; i < s->nbJetons; i += 1) {
        if (s->jetons[i*2] == x) return true;
    }
    return false;
}

bool coordonneeYestDansSection (int y, Section* s) {
    for (int i = 0; i < s->nbJetons; i += 1) {
        if (s->jetons[i*2+1] == y) return true;
    }
    return false;
}

int directionSectionDiagonale (Section* s) {
    if (s->type != DIAGONALE) return 0;
    if (s->jetons[1] < s->jetons[3]) return 2;
    return 1;
}

bool estSectionDoubleVictoire (Board *b, Section* s) {
    if (s->nbJetons != 3) return false;
    if (s->type == COLONNE) return false;
    if (coordonneeYestDansSection(0, s) || coordonneeYestDansSection(6, s)) return false;
    if (s->type == LIGNE) {
        if (s->ext1 || s->ext2) return false;
        if (s->jetons[0] == 5) return true;
        if (b->jetons[s->e1[0]+1][s->e1[1]-1] && b->jetons[s->e2[0]+1][s->e2[1]+1]) return true;
        return false;
    }
    if (coordonneeXestDansSection(0, s) || coordonneeXestDansSection(5, s)) return false;
    if (s->type == DIAGONALE) {
        if (directionSectionDiagonale(s) == 1) {
            if (b->jetons[s->jetons[0]+1][s->jetons[1]+1] || b->jetons[s->jetons[4]-1][s->jetons[5]-1]) return false;
            if ((s->jetons[0]+1 == 5 || b->jetons[s->jetons[0]+2][s->jetons[1]+1]) && b->jetons[s->jetons[4]][s->jetons[5]-1]) return true;
        } else if (directionSectionDiagonale(s) == 2) {
            if (b->jetons[s->jetons[0]+1][s->jetons[1]-1] || b->jetons[s->jetons[4]-1][s->jetons[5]+1]) return false;
            if ((s->jetons[0]+1 == 5 || b->jetons[s->jetons[0]+2][s->jetons[1]-1]) && b->jetons[s->jetons[4]][s->jetons[5]+1]) return true;
        }
    }
    return false;
}

bool chercheSectionsDoubleVictoire (Board *b, TableauSections *ts) {
    for (int i = 0; i < ts->nbSections; i += 1) {
        if (estSectionDoubleVictoire(b, ts->s[i])) return true;
    }
    return false;
}

void afficherSection (Board *b, Section* s) {
    printf("Couleur : %d, Type : %u, nombre de jetons : %d, double victoire = %d\n", s->couleur, s->type, s->nbJetons, estSectionDoubleVictoire(b, s));
    printf("Extremite 1 = %d, Extremite 2 = %d\n", s->ext1, s->ext2);
    for (int i = 0; i < s->nbJetons; i += 1) {
        printf("Jeton n°%d : x = %d, y = %d\n", i, s->jetons[i*2], s->jetons[i*2+1]);
    }
}

void afficherTableauSections (Board *b,TableauSections *ts){
    for (int i = 0; i < ts->nbSections; i += 1) {
        printf("Section n°%d ==> ", i+1);
        afficherSection(b, ts->s[i]);
    }
}

bool cherchePointExtremiteSection (int x, int y, Section *s) {
    if (s->e1[0] == x && s->e1[1] == y) return true;
    if (s->e2[0] == x && s->e2[1] == y) return true;
    return false;
}

bool emplacementInterditAdversaire (Board *b, Section *s) {
    if (s->type == COLONNE) return false;
    if (s->nbJetons == 3) return false;
    if (s->type == LIGNE && s->jetons[0] == 5) return false;
    int x = b->dernierJeton[0], y = b->dernierJeton[1];
    int numExt;
    if (s->e1[0] == x && s->e1[1] == y) numExt = 1;
    else numExt = 2;
    Section *newS;
    // Crée la section selon si c'est une ligne ou bien une des 2 sortes de diagonales. Différences dans l'ordre des points
    if (numExt == 1) newS = creerSection(s->type, s->couleur, s->nbJetons, s->jetons[0], s->jetons[1], s->jetons[2], s->jetons[3], x, y);
    else newS = creerSection(s->type, s->couleur, s->nbJetons, x, y, s->jetons[0], s->jetons[1], s->jetons[2], s->jetons[3]);
    calculeExtremite(b, &newS);
    if (newS->e1[0] != 5 && !newS->ext1) { //extrémité 1
        if (!b->jetons[newS->e1[0]+1][newS->e1[1]]) { //pas de jeton dans la case sous l'extrémité
            if (newS->e1[0]+1 == 5) goto vrai; //mais c'est le bas après
            if (b->jetons[newS->e1[0]+2][newS->e1[1]]) goto vrai; //mais il y a un jeton sur en dessous
        }
    }
    if (newS->e2[0] != 5 && !newS->ext2) { //extrémité 1
        if (!b->jetons[newS->e2[0]+1][newS->e2[1]]) { //pas de jeton dans la case sous l'extrémité
            if (newS->e2[0]+1 == 5) goto vrai; //mais c'est le bas après
            if (b->jetons[newS->e2[0]+2][newS->e2[1]]) goto vrai; //mais il y a un jeton sur en dessous
        }
    }
    free(newS);
    return false;
    vrai:
        free(newS);
        return true;
}

int chercheSectionsDernierJeton (Board *b, TableauSections *ts) {
    int res = 0;
    int x = b->dernierJeton[0];
    int y = b->dernierJeton[1];
    int couleur = b->tour;
    for (int i = 0; i < ts->nbSections; i += 1) {
        if (!chercheExtremiteSection(x, y, ts->s[i])) continue;
        if (ts->s[i]->nbJetons == 1) continue;
        if (ts->s[i]->couleur == couleur) {
            if (ts->s[i]->nbJetons == 2) continue;
            if (ts->s[i]->ext1 && ts->s[i]->ext2) continue;
            if (ts->s[i]->type == COLONNE && x == 0) continue;
            if (!res) res = 2;
            else if (res == 1) res = 4;
            if (emplacementInterditAdversaire(b, ts->s[i])) {
                if (res == 2) res = 3;
                else if (res == 1 || res == 4) res = 5;
            }
        } else {
            if (!res) res = 1;
            else if (res == 2) res = 4;
            else if (res == 3) res = 5;
        }
    }
    return res;
}

void predictionCoups (int **grade, Jeu* jeu) {
    int couleur = jeu->b->tour;
    int cgA = (couleur == 1) ? 3 : 1; //case coup gagnant adversaire
    int cgJ = (couleur == 1) ? 1 : 3; //case coup gagnant joueur
    Jeu* copy = copyJeu(jeu);
    for (int i = 0; i < 7; i += 1) { //l'ia joue dans chaque colonne
        bool ok = jouerJeton(&copy->b, i);
        if (!ok) { //colonne pleine
            (*grade)[i] = -1;
            continue;
        }
        TableauSections* ts = creerTableauSections();
        chercherSections(copy->b, &ts);
        if (chercheSectionsDoubleVictoire(copy->b, ts)) { //si vous vous créez une double victoire en jouant là
            (*grade)[i] = 5;
            annuleCoup(&copy->b);
            freeTableauSections(ts);
            continue;
        }
        freeTableauSections(ts);

        int* cg = coupGagnant(copy->b);
        if (cg[cgA] != -1) { //coup gagnant à l'adversaire
            (*grade)[i] = 1;
            annuleCoup(&copy->b);
            free(cg);
            continue;
        } else if (cg[cgJ] != -1) { //coup gagnant pour vous
            (*grade)[i] = 0;
            annuleCoup(&copy->b);
            free(cg);
            continue;
        }
        free(cg);

        ts = creerTableauSections();
        chercherSections(copy->b, &ts);
        int obs = chercheSectionsDernierJeton(copy->b ,ts); //évaluation avec toutes les sections
        switch (obs) {
            case 0:
                (*grade)[i] = 2;
                break;
            case 1:
                (*grade)[i] = 3;
                break;
            case 2:
                (*grade)[i] = 3;
                break;
            case 3:
                (*grade)[i] = 5;
                break;
            case 4:
                (*grade)[i] = 4;
                break;
            case 5:
                (*grade)[i] = 5;
                break;
            default:
                fprintf(stderr, "C'est pas normal. Touchez pas au code sinon ça marche plus.");
                break;
        }
        freeTableauSections(ts);

        Jeu* copy2 = copyJeu(copy);
        swapTour(&copy2->b);
        //cherche si l'adversaire peut créer se créer une double victoire au coup suivant si vous jouez dans la colonne i
        for (int j = 0; j < 7; j += 1) {
            jouerJeton(&copy2->b, j);
            ts = creerTableauSections();
            chercherSections(copy2->b, &ts);
            if (chercheSectionsDoubleVictoire(copy2->b, ts)) {
                (*grade)[i] = 1;
                freeTableauSections(ts);
                break;
            }
            annuleCoup(&copy2->b);
            freeTableauSections(ts);
        }
        freeJeu(copy2);
        annuleCoup(&copy->b);
    }
    freeJeu(copy);
}