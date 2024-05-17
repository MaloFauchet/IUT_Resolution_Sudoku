/**
 * \file RESOLUTION-2.c
 * \brief Programme de resolution de sudoku
 * \author PFRANGER Matheo, FAUCHET Malo
 * \date 28 Janvier 2024
 * \version 1.0
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

/**
 * \def n
 * \brief Taille d un bloc de la grille
*/
#define n 4

/**
 * \def TAILLE
 * \brief Taille de la grille (n*n)
*/
#define TAILLE (n*n)


/**
 * \struct tCase1
 * \brief Structure representant une case de la grille
 * 
 * La structure tCase1 contient :
 * - valeur : la valeur de la case
 * - candidats : un tableau contenant les candidats possibles pour la case
 * - nbCandidats : le nombre de candidats possibles pour la case
*/
typedef struct {
    int valeur;
    int candidats[TAILLE];
    int nbCandidats;
} tCase1;

// candidat_grille est un tableau de TAILLE lignes et TAILLE colonnes contenant les candidats possibles pour chaque case
typedef tCase1 candidat_grille[TAILLE][TAILLE];

// tGrille est un tableau de TAILLE lignes et TAILLE colonnes contenant la grille de jeu initiale
typedef int tGrille[TAILLE][TAILLE];


/**
 * \def CELLULE_VIDE
 * \brief Caractere representant une cellule vide
*/
const char CELLULE_VIDE = '.' ;

// Backtracking functions
bool absentSurLigne(int valeur, candidat_grille grille_candidat, int numLigne);
bool absentSurColonne(int valeur, candidat_grille grille_candidat, int numColonne);
bool absentSurBloc(int valeur, candidat_grille grille_candidat, int numLigne, int numColonne);
bool backtracking(candidat_grille grille_candidat, int numCase);
int premiereCaseVide(candidat_grille grille_candidat);

// Candidats functions
void initGrilleCandidat(candidat_grille grille_candidat, tGrille grille);
void initCandidat(candidat_grille grille_candidat);
bool possible(candidat_grille grille, int num_ligne, int num_colonne, int valeur);
void ajouterCandidat(tCase1 *laCase, int valeur);
void retirerCandidat(tCase1 *laCase, int valeur);
bool estCandidat(tCase1 la_case, int valeur);

// Singleton nu
bool checkSingletonNu(candidat_grille grille);

// Singleton cachee
void retirerCandidatLigne(candidat_grille grille_candidat, int num_lig, int valeur);
void retirerCandidatColonne(candidat_grille grille, int num_col, int valeur);
void retirerCandidatRegion(candidat_grille grille_candidat, int num_ligne, int num_colonne, int valeur);
void checkSingletonCacheeRegion(candidat_grille grille, int num_ligne, int num_colonne, bool *action_effectuee);
void checkSingletonCacheeLigne(candidat_grille grille_candidat, int num_ligne, bool *action_effectuee);
void checkSingletonCacheeColonne(candidat_grille grille_candidat, int num_col, bool *action_effectuee);
bool checkSingletonCachee(candidat_grille grille_candidat);

// Misc functions
char chargerGrille(tGrille grille);
int nombreChiffre(int nombre);
void afficherEspaces(int nombre_espaces);
void afficherLigneSeparatrice(int nombre_espaces);
void afficherGrille(candidat_grille grille_candidat);


/*****************************************************
 *                PROGRAMME PRINCIPAL                *
 *****************************************************/

int main() {
    tGrille grille;
    candidat_grille grille_candidat;

    chargerGrille(grille);
    initGrilleCandidat(grille_candidat, grille);
    initCandidat(grille_candidat);

    bool action_effectuee = true;
    bool tmp = true;
    bool end_of_game = false;

    printf("Grille initial\n");
    afficherGrille(grille_candidat);

    clock_t start = clock();

    while (end_of_game != true) {
        // afficherGrille(grille_candidat);
        tmp = checkSingletonNu(grille_candidat);
        if (!tmp)
            action_effectuee = false;
        while (tmp) {
            action_effectuee = true;
            tmp = checkSingletonNu(grille_candidat);
        }
        if (!action_effectuee) {
            tmp = checkSingletonCachee(grille_candidat);
            if (!tmp)
                end_of_game = true;
            while (tmp) {
                action_effectuee = true;
                tmp = checkSingletonCachee(grille_candidat);
            }
        }
    }

    backtracking(grille_candidat, premiereCaseVide(grille_candidat));

    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    
    printf("Grille finale\n");
    afficherGrille(grille_candidat);
    printf("Resolution de la grille en %.6f secondes\n", time_spent);

    return EXIT_SUCCESS;
}

/**
 * \fn bool absentSurLigne(int valeur, candidat_grille grille_candidat, int numLigne
 * \brief Verifie si une valeur est absente sur une ligne
 * \param valeur Valeur dont on veut verifier l absence sur la ligne
 * \param grille_candidat Grille de jeu
 * \param numLigne Numero de la ligne de la case selectionnee
 * 
 * \return true si la valeur est absente sur la ligne, false sinon
*/
bool absentSurLigne(int valeur, candidat_grille grille_candidat, int numLigne) {
    int i;
    bool absent = true;
    i = 0;
    while (i < TAILLE && absent) {
        if (grille_candidat[numLigne][i].valeur == valeur) {
            absent = false;
        }
        i++;
    }
    return absent;
}

/**
 * \fn bool absentSurColonne(int valeur, candidat_grille grille_candidat, int numColonne)
 * \brief Verifie si une valeur est absente sur une colonne
 * \param valeur Valeur dont on veut verifier l absence sur la colonne
 * \param grille_candidat Grille de jeu
 * \param numColonne Numero de la colonne de la case selectionnee
 * 
 * \return true si la valeur est absente sur la colonne, false sinon
*/
bool absentSurColonne(int valeur, candidat_grille grille_candidat, int numColonne) {
    int i;
    bool absent = true;
    i = 0;
    while (i < TAILLE && absent) {
        if (grille_candidat[i][numColonne].valeur == valeur) {
            absent = false;
        }
        i++;
    }
    return absent;
}

/**
 * \fn bool absentSurBloc(int valeur, candidat_grille grille_candidat, int numLigne, int numColonne)
 * \brief Verifie si une valeur est absente sur un bloc
 * \param valeur Valeur dont on veut verifier l absence sur le bloc
 * \param grille_candidat Grille de jeu
 * \param numLigne Numero de la ligne de la case selectionnee
 * \param numColonne Numero de la colonne de la case selectionnee
 * 
 * \return true si la valeur est absente sur le bloc, false sinon
*/
bool absentSurBloc(int valeur, candidat_grille grille_candidat, int numLigne, int numColonne) {
    int startingRow = numLigne - (numLigne%n);
    int startingCol = numColonne - (numColonne%n);

    int i, j;
    bool absent = true;
    i = startingRow;
    while (i < startingRow + n && absent) {
        j = startingCol;
        while (j < startingCol + n && absent) {
            if (grille_candidat[i][j].valeur == valeur) {
                absent = false;
            }
            j++;
        }
        i++;
    }
    return absent;
}

/**
 * \fn bool backtracking(candidat_grille grille_candidat, int numCase)
 * \brief Resout la grille de jeu par backtracking
 * \param grille_candidat Grille de jeu
 * \param numCase Numero de la case a traiter
 * 
 * \return true si la grille est resolue, false sinon
 * 
 * Cette fonction resout la grille de jeu par backtracking.
 * Elle verifie si la case est vide, si oui, elle teste toutes les valeurs possibles
 * et si une valeur est autorisee, elle l inscrit dans la case et passe a la case suivante.
 * Si aucune valeur n est autorisee, elle revient en arriere et teste une autre valeur.
 * Si la case n est pas vide, elle passe a la case suivante.
*/
bool backtracking(candidat_grille grille_candidat, int numCase){
    int lig, col;
    bool res = false;

    if(numCase == TAILLE*TAILLE){
        // On a traité tout les cases, la grille est résolue
        res = true;
    } else {
        // On récupère les "coordonnées" de la case
        lig = numCase / TAILLE;
        col = numCase % TAILLE;
        if (grille_candidat[lig][col].valeur != 0) {
            // La case n'est pas vide, on passe à la suivante
            // (appel récursif)
            res = backtracking(grille_candidat, numCase+1);
        } else {
            for (int val = 1; val <= TAILLE; val++) {
                if (absentSurLigne(val, grille_candidat, lig)
                && absentSurColonne(val, grille_candidat, col)
                && absentSurBloc(val, grille_candidat, lig, col)) {
                    // Si la valeur est autorisée, on l'inscrit dans la case
                    grille_candidat[lig][col].valeur = val;
                    // Et on passe à la case suivante : appel récursif
                    // pour voir si ce choix est bon par la suite
                    if (backtracking(grille_candidat, numCase+1) == true) {
                        res = true;
                    } else {
                        grille_candidat[lig][col].valeur = 0;
                    }
                }
            }
        } 
    }
    return res;
}

/**
 * \fn int premiereCaseVide(candidat_grille grille_candidat)
 * \brief Determine la premiere case vide de la grille
 * \param grille_candidat Grille de jeu
 * 
 * \return Numero de la premiere case vide de la grille
 * 
 * Cette fonction determine la premiere case vide de la grille
 * en parcourant la grille de gauche a droite et de haut en bas.
 * Si aucune case vide n est trouvee, la fonction retourne -1.
*/
int premiereCaseVide(candidat_grille grille_candidat) {
    int i, j;
    bool trouve = false;
    i = 0;
    while (i < TAILLE && !trouve) {
        j = 0;
        while (j < TAILLE && !trouve) {
            if (grille_candidat[i][j].valeur == 0) {
                trouve = true;
            }
            j++;
        }
        i++;
    }
    return (i-1)*TAILLE + (j-1);
}

/*****************************************************
 *                FONCTIONS CANDIDATS                *
 *****************************************************/

/** 
 * \fn void afficherCandidats(tCase1 laCase)
 * \brief Affiche les candidats d une case
 * \param laCase Case dont on veut afficher les candidats
 * 
 * Cette fonction affiche les candidats d une case en parcourant
 * la liste des candidats de la case.
*/
void afficherCandidats(tCase1 laCase) {
    int i;
    for (i = 0; i < laCase.nbCandidats; i++) {
        printf("%d ", laCase.candidats[i]);
    }
    printf("\n");
}

/**
 * \fn void ajouterCandidat(tCase1 *laCase, int valeur)
 * \brief Ajoute un candidat a une case
 * \param laCase Case a laquelle on veut ajouter un candidat
 * \param valeur Valeur a ajouter a la liste des candidats de la case
 * 
 * Cette fonction ajoute un candidat a une case en verifiant si la valeur
 * n est pas deja presente dans la liste des candidats de la case.
*/
void ajouterCandidat(tCase1 *laCase, int valeur) {
    laCase->candidats[laCase->nbCandidats] = valeur;
    laCase->nbCandidats++;
}

/**
 * \fn void retirerCandidat(tCase1 *laCase, int valeur)
 * \brief Retire un candidat d une case
 * \param laCase Case dont on veut retirer un candidat
 * \param valeur Valeur a retirer de la liste des candidats de la case
 * 
 * Cette fonction retire un candidat d une case en verifiant si la valeur
 * est presente dans la liste des candidats de la case.
*/
void retirerCandidat(tCase1 *laCase, int valeur) {
    int i, j;
    bool trouve = false;
    i = 0;

    if (laCase->valeur != 0)
        return;

    while (i < laCase->nbCandidats && !trouve) {
        if (laCase->candidats[i] == valeur) {
            j = i;
            trouve = true;
            laCase->nbCandidats--;
            while (j != laCase->nbCandidats) {
                laCase->candidats[j] = laCase->candidats[j+1];
                j++;
            }
        }
        i++;
    }
}

/**
 * \fn bool estCandidat(tCase1 laCase, int valeur)
 * \brief Verifie si une valeur est un candidat d une case
 * \param laCase Case dont on veut verifier si la valeur est un candidat
 * \param valeur Valeur dont on veut verifier si elle est un candidat de la case
 * 
 * \return true si la valeur est un candidat de la case, false sinon
 * 
 * Cette fonction verifie si la valeur est un candidat de la case en verifiant
 * si la valeur est presente dans la liste des candidats de la case.
*/
bool estCandidat(tCase1 laCase, int valeur) {
    int i;
    bool trouve = false;
    i = 0;
    while (i < laCase.nbCandidats && !trouve) {
        if (laCase.candidats[i] == valeur) {
            trouve = true;
        }
        i++;
    }
    return trouve;
}

/**
 * \fn int nbCandidats(tCase1 laCase)
 * \brief Retourne le nombre de candidats d une case
 * \param laCase Case dont on veut connaitre le nombre de candidats
 * 
 * \return Nombre de candidats de la case
*/
int nbCandidats(tCase1 laCase) {
    return laCase.nbCandidats;
}


/**
 * \fn void initGrilleCandidat(candidat_grille grilleCandidat, tGrille grille)
 * \brief Initialise la grille de candidats
 * \param grille_candidat Grille de candidats
 * \param grille Grille de jeu
 * 
 * Cette fonction initialise la grille de candidats en copiant la grille de jeu
 * dans la grille de candidats et en initialisant le nombre de candidats de chaque case a 0.
*/
void initGrilleCandidat(candidat_grille grille_candidat, tGrille grille) {
    for (int i = 0; i < TAILLE; i++) {
        for (int j = 0; j < TAILLE; j++) {
            grille_candidat[i][j].valeur = grille[i][j];
            grille_candidat[i][j].nbCandidats = 0;
        }
    }
}

/**
 * \fn bool possible(tGrille grille, int numLigne, int numColonne, int valeur)
 * \brief Verifie si une valeur peut etre inseree dans une case
 * \param grille Grille de jeu
 * \param numLigne Numero de la ligne de la case selectionnee
 * \param numColonne Numero de la colonne de la case selectionnee
 * \param valeur Valeur a inserer dans la case selectionnee
 * 
 * \return true si la valeur peut etre inseree, false sinon
 * 
 * Cette fonction verifie si la valeur peut etre inseree dans la case
 * selectionnee en verifiant si la valeur n est pas deja presente dans
 * la ligne, la colonne ou le bloc de la case selectionnee.
*/
bool possible(candidat_grille grille, int numLigne, int numColonne, int valeur) {
    int i, j;
    bool possible = true;

    // check colonne
    i = 0;
    while (i < TAILLE && possible) {
        if (grille[numLigne][i].valeur == valeur) {
            // printf("La valeur %d ne peut pas être placée dans la ligne %d\ncar elle est déjà présente dans la même ligne à la colonne %d\n", valeur, numLigne+1, i+1);
            possible = false;
        }
        i++;
    }

    // check ligne
    i = 0;
    while (i < TAILLE && possible) {
        if (grille[i][numColonne].valeur == valeur) {
            // printf("La valeur %d ne peut pas être placée dans la colonne %d\ncar elle est déjà présente dans la même colonne à la ligne %d\n", valeur, numColonne+1, i+1);
            possible = false;
        }
        i++;
    }

    // check region
    int startingRow = numLigne - (numLigne%n);
    int startingCol = numColonne - (numColonne%n);

    i = startingRow;
    while (i < startingRow + n && possible) {
        j = startingCol;
        while (j < startingCol + n && possible) {
            if (grille[i][j].valeur == valeur) {
                // printf("La valeur %d ne peut pas être placée dans cette région\ncar elle est déjà présente dans la même région\n", valeur);
                possible = false;
            }
            j++;
        }
        i++;
    }

    return possible;
}

/**
 * \fn void initCandidat(candidat_grille grille_candidat)
 * \brief Initialise les candidats de chaque case de la grille.
 * \param grille_candidat Grille de candidats
 * 
 * Cette fonction initialise les candidats de chaque case de la grille
 * en verifiant si chaque valeur est possible dans chaque case.
*/
void initCandidat(candidat_grille grille_candidat) {
    int num_ligne, num_colonne, valeur_testee;
    
    // parcours de la grille
    for (num_ligne = 0; num_ligne < TAILLE; num_ligne++) {
        for (num_colonne = 0; num_colonne < TAILLE; num_colonne++) {
            // check si la case est vide
            if (grille_candidat[num_ligne][num_colonne].valeur == 0) {
                // Si oui, on réinitialise le nombre de candidat et on teste toutes les valeurs possibles
                grille_candidat[num_ligne][num_colonne].nbCandidats = 0;
                for (valeur_testee = 1; valeur_testee <= TAILLE; valeur_testee++) {
                    if (possible(grille_candidat, num_ligne, num_colonne, valeur_testee)) {
                        ajouterCandidat(&grille_candidat[num_ligne][num_colonne], valeur_testee);
                    }
                }
            }
        }
    }
}

/**
 * \fn void retirerCandidatLigne(candidat_grille grille_candidat, int num_ligne, int valeur)
 * \brief Retire un candidat d une ligne
 * \param grille_candidat Grille de candidats
 * \param num_ligne Numero de la ligne de la case selectionnee
 * \param valeur Valeur a inserer dans la case selectionnee
 * 
 * Cette fonction retire un candidat d une ligne en verifiant si la valeur
 * est presente dans la ligne de la case selectionnee.
*/
void retirerCandidatLigne(candidat_grille grille_candidat, int num_ligne, int valeur) {
    for (int i = 0; i < TAILLE; i++) {
        retirerCandidat(&grille_candidat[num_ligne][i], valeur);
    }
}

/**
 * \fn void retirerCandidatColonne(candidat_grille grille_candidat, int num_colonne, int valeur)
 * \brief Retire un candidat d une colonne
 * \param grille_candidat Grille de candidats
 * \param num_colonne Numero de la colonne de la case selectionnee
 * \param valeur Valeur a inserer dans la case selectionnee
 * 
 * Cette fonction retire un candidat d une colonne en verifiant si la valeur
 * est presente dans la colonne de la case selectionnee.
*/
void retirerCandidatColonne(candidat_grille grille_candidat, int num_colonne, int valeur) {
    for (int i = 0; i < TAILLE; i++) {
        retirerCandidat(&grille_candidat[i][num_colonne], valeur);
    }
}

/**
 * \fn void retirerCandidatRegion(candidat_grille grille_candidat, int num_ligne, int num_colonne, int valeur)
 * \brief Retire un candidat d une region
 * \param grille_candidat Grille de candidats
 * \param num_ligne Numero de la ligne de la case selectionnee
 * \param num_colonne Numero de la colonne de la case selectionnee
 * \param valeur Valeur a inserer dans la case selectionnee
 * 
 * Cette fonction retire un candidat d une region en verifiant si la valeur
 * est presente dans la region de la case selectionnee.
*/
void retirerCandidatRegion(candidat_grille grille_candidat, int num_ligne, int num_colonne, int valeur) {
    int startingRow = num_ligne - (num_ligne%n);
    int startingCol = num_colonne - (num_colonne%n);

    for (int i = startingRow; i < startingRow + n; i++) {
        for (int j = startingCol; j < startingCol + n; j++) {
            retirerCandidat(&grille_candidat[i][j], valeur);
        }
    }
}

/**
 * \fn bool checkSingletonNu(candidat_grille grille_candidat)
 * \brief Verifie si une case a un seul candidat
 * \param grille_candidat Grille de candidats
 * 
 * \return true si il y a eu au moins un singleton, false sinon
 * 
 * Cette fonction passe à travers chaque case de la grille de candidats.
 * 
 * Elle verifie si une case a un seul candidat en verifiant
 * si le nombre de candidats de la case est egal a 1.
 * Si c est le cas, la fonction affecte la valeur du candidat a la case
 * et retire le candidat de la liste des candidats de la case.
*/
bool checkSingletonNu(candidat_grille grille_candidat) {
    bool action_effectuee = false;
    for (int i = 0; i < TAILLE; i++) {
        for (int j = 0; j < TAILLE; j++) {
            if ((grille_candidat[i][j].valeur == 0) && (nbCandidats(grille_candidat[i][j]) == 1)) {
                action_effectuee = true;

                // printf("Singleton nu (%d) trouve dans la case (%d, %d)\n", grille_candidat[i][j].candidats[0], i+1, j+1);

                grille_candidat[i][j].valeur = grille_candidat[i][j].candidats[0];
                grille_candidat[i][j].nbCandidats = 0;

                retirerCandidatLigne(grille_candidat, i, grille_candidat[i][j].valeur);
                retirerCandidatColonne(grille_candidat, j, grille_candidat[i][j].valeur);
                retirerCandidatRegion(grille_candidat, i, j, grille_candidat[i][j].valeur);
            }
        }
    }
    return action_effectuee;
}

/**
 * \fn void checkSingletonCacheeRegion(candidat_grille grille_candidat, int num_ligne, int num_colonne, bool *action_effectuee)
 * \brief Verifie si une case a un seul candidat dans sa region
 * \param grille_candidat Grille de candidats
 * \param num_ligne Numero de la ligne de la case selectionnee
 * \param num_colonne Numero de la colonne de la case selectionnee
 * \param action_effectuee Pointeur vers un booleen qui indique si une action a ete effectuee
 * \param nb_candidats_elimines Pointeur vers un float qui indique le nombre de candidats elimines
 * \param nb_cases_remplies Pointeur vers un float qui indique le nombre de cases remplies
 * 
 * Cette fonction verifie si une case a un seul candidat dans sa region en verifiant
 * si le nombre de candidats de la case est egal a 1.
 * Si c est le cas, la fonction affecte la valeur du candidat a la case
 * et retire le candidat de la liste des candidats de la case.
*/
void checkSingletonCacheeRegion(candidat_grille grille_candidat, int num_ligne, int num_colonne, bool *action_effectuee) {
    int startingRow = num_ligne - (num_ligne%n);
    int startingCol = num_colonne - (num_colonne%n);
    
    int nb_occurences[TAILLE] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

    // compte le nombre d occurences de chaque candidat dans la region
    for (int i = startingRow; i < startingRow+n; i++) {
        for (int j = startingCol; j < startingCol+n; j++) {
            for (int k = 0; k < nbCandidats(grille_candidat[i][j]); k++) {
                nb_occurences[grille_candidat[i][j].candidats[k]-1]++;
            }
        }
    }

    // check si un candidat n apparait qu une seule fois dans la region
    for (int i = 0; i < TAILLE; i++) {
        if (nb_occurences[i] == 1) {
            *action_effectuee = true;
            // printf("Singleton cachee (%d) trouve dans la region (%d, %d)\n", i+1, num_ligne+1, num_colonne+1);
            // cherche la case qui contient le candidat
            for (int j = startingRow; j < startingRow+n; j++) {
                for (int k = startingCol; k < startingCol+n; k++) {
                    if (estCandidat(grille_candidat[j][k], i+1)) {
                        grille_candidat[j][k].valeur = i+1;
                        grille_candidat[j][k].nbCandidats = 0;

                        retirerCandidatLigne(grille_candidat, j, grille_candidat[j][k].valeur);
                        retirerCandidatColonne(grille_candidat, k, grille_candidat[j][k].valeur);
                    }
                }
            }
        }
    }
}

/**
 * \fn void checkSingletonCacheeLigne(candidat_grille grille_candidat, int num_ligne, bool *action_effectuee)
 * \brief Verifie si une case a un seul candidat dans sa ligne
 * \param grille_candidat Grille de candidats
 * \param num_ligne Numero de la ligne de la case selectionnee
 * \param action_effectuee Pointeur vers un booleen qui indique si une action a ete effectuee
 * \param nb_candidats_elimines Pointeur vers un float qui indique le nombre de candidats elimines
 * \param nb_cases_remplies Pointeur vers un float qui indique le nombre de cases remplies
 * 
 * Cette fonction verifie si une case a un seul candidat dans sa ligne en verifiant
 * si le nombre de candidats de la case est egal a 1.
 * Si c est le cas, la fonction affecte la valeur du candidat a la case
 * et retire le candidat de la liste des candidats de la case.
*/
void checkSingletonCacheeLigne(candidat_grille grille_candidat, int num_ligne, bool *action_effectuee) {
    int nb_occurences[TAILLE] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

    // compte le nombre d occurences de chaque candidat dans la ligne
    for (int i = 0; i < TAILLE; i++) {
        for (int j = 0; j < nbCandidats(grille_candidat[num_ligne][i]); j++) {
            nb_occurences[grille_candidat[num_ligne][i].candidats[j]-1]++;
        }
    }

    // check si un candidat n apparait qu une seule fois dans la ligne
    for (int i = 0; i < TAILLE; i++) {
        if (nb_occurences[i] == 1) {
            *action_effectuee = true;
            // printf("Singleton cachee (%d) trouvee dans la ligne %d\n", i+1, num_ligne+1);
            // cherche la case qui contient le candidat
            for (int j = 0; j < TAILLE; j++) {
                if (estCandidat(grille_candidat[num_ligne][j], i+1)) {
                    grille_candidat[num_ligne][j].valeur = i+1;
                    grille_candidat[num_ligne][j].nbCandidats = 0;

                    retirerCandidatColonne(grille_candidat, j, grille_candidat[num_ligne][j].valeur);
                    retirerCandidatRegion(grille_candidat, num_ligne, j, grille_candidat[num_ligne][j].valeur);
                }
            }
        }
    }
}

/**
 * \fn void checkSingletonCacheeColonne(candidat_grille grille_candidat, int num_colonne, bool *action_effectuee)
 * \brief Verifie si une case a un seul candidat dans sa colonne
 * \param grille_candidat Grille de candidats
 * \param num_colonne Numero de la colonne de la case selectionnee
 * \param action_effectuee Pointeur vers un booleen qui indique si une action a ete effectuee
 * \param nb_candidats_elimines Pointeur vers un float qui indique le nombre de candidats elimines
 * \param nb_cases_remplies Pointeur vers un float qui indique le nombre de cases remplies
 * 
 * Cette fonction verifie si une case a un seul candidat dans sa colonne en verifiant
 * si le nombre de candidats de la case est egal a 1.
 * Si c est le cas, la fonction affecte la valeur du candidat a la case
 * et retire le candidat de la liste des candidats de la case.
*/
void checkSingletonCacheeColonne(candidat_grille grille_candidat, int num_colonne, bool *action_effectuee) {
    int nb_occurences[TAILLE] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

    // compte le nombre d occurences de chaque candidat dans la colonne
    for (int i = 0; i < TAILLE; i++) {
        for (int j = 0; j < nbCandidats(grille_candidat[i][num_colonne]); j++) {
            nb_occurences[grille_candidat[i][num_colonne].candidats[j]-1]++;
        }
    }

    // check si un candidat n apparait qu une seule fois dans la colonne
    for (int i = 0; i < TAILLE; i++) {
        if (nb_occurences[i] == 1) {
            *action_effectuee = true;
            // printf("Singleton cachee (%d) trouvee dans la colonne %d\n", i+1, num_colonne+1);
            // cherche la case qui contient le candidat
            for (int j = 0; j < TAILLE; j++) {
                if (estCandidat(grille_candidat[j][num_colonne], i+1)) {
                    grille_candidat[j][num_colonne].valeur = i+1;
                    grille_candidat[j][num_colonne].nbCandidats = 0;

                    retirerCandidatLigne(grille_candidat, j, grille_candidat[j][num_colonne].valeur);
                    retirerCandidatRegion(grille_candidat, j, num_colonne, grille_candidat[j][num_colonne].valeur);
                    break;
                }
            }
        }
    }
}

/**
 * \fn bool checkSingletonCachee(candidat_grille grille_candidat)
 * \brief Verifie si une case a un seul candidat dans sa ligne, colonne ou region
 * \param grille_candidat Grille de candidats
 * \param nb_candidats_elimines Pointeur vers un float qui indique le nombre de candidats elimines
 * \param nb_cases_remplies Pointeur vers un float qui indique le nombre de cases remplies
 * 
 * \return true si il y a eu au moins un singleton cachee, false sinon
*/
bool checkSingletonCachee(candidat_grille grille_candidat) {
    bool action_effectuee = false;

    for (int i = 0; i < TAILLE; i++) {
        checkSingletonCacheeLigne(grille_candidat, i, &action_effectuee);
    }
    for (int i = 0; i < TAILLE; i++) {
        checkSingletonCacheeColonne(grille_candidat, i, &action_effectuee);
    }
    // check regions
    for (int i = 0; i < TAILLE; i+=n) {
        for (int j = 0; j < TAILLE; j+=n) {
            checkSingletonCacheeRegion(grille_candidat, i, j, &action_effectuee);
        }
    }
    return action_effectuee;
}

/*****************************************************
 *                  FONCTIONS AUTRES                 *
 *****************************************************/

/**
 * \fn void chargerGrille(tGrille grille)
 * \brief Charge une grille de jeu a partir d un fichier
 * \param grille Grille de jeu a initialiser
 * 
 * \return Le charactere correspondant au numero de la grille
 * La fonction charge une grille de jeu a partir d un fichier
 * dont le nom est saisi au clavier.
 * Cette fonction arrete le programme avec le code de sortie 1 
 * si le dossier 'grilles/' n est pas present ou si le fichier n est pas trouve
*/
char chargerGrille(tGrille grille){
    FILE * f;
    char nomFichier[30];  // chaine de caracteres contenant le nom du fichier de grille a initialiser
    char numFic;  // le numero du fichier de grille a initialiser */

    // test si le dossier ./grilles est present
    if (access("./grilles", 0) == 0) {
        
        // choix du fichier a utiliser afin d initialiser la grille de jeu
        printf("Choisissez un numéro de grille entre A et D : ");
        scanf("%c", &numFic);

        sprintf(nomFichier, "grilles/MaxiGrille%c.sud", numFic);

        // utilisation du fichier
        f = fopen(nomFichier, "rb");
        if (f==NULL){
            printf("ERREUR lors du chargement du fichier.\n");
            printf("\tAssurez-vous que le dossier \'grilles/\' soit présent au même endroit que le programme,\n");
            printf("\tet qu'il contienne les fichier \'MaxiGrille_.sud\', où \'_\' est un nombre entre de 1 à 10.\n");
            exit(EXIT_FAILURE);
        } else {
            fread(grille, sizeof(int), TAILLE*TAILLE, f);
        }
        fclose(f);
    } 
    else {
        printf("ERREUR. Le dossier \'grilles/\' est introuvable.\n");
        printf("\tAssurez-vous que le dossier \'grilles/\' soit présent au même endroit que le programme,\n");
        printf("\tet qu'il contienne les fichier \'MaxiGrille_.sud\', où \'_\' est un nombre entre de 1 à 10.\n");
        exit(EXIT_FAILURE);
    }
    return numFic;
}

/**
 * \fn int nombreChiffre(int nombre)
 * \brief Determine le nombre de chiffres d un nombre
 * \param nombre Nombre dont on veut connaitre le nombre de chiffres
 * 
 * \return Nombre de chiffres du nombre
 * 
 * Cette fonction calcule le nombre de chiffres d un nombre en
 * divisant le nombre par 10 jusqu a ce que le nombre soit egal a 0.
*/
int nombreChiffre(int nombre) {
    int nombre_chiffre_tmp = 0;
    while (nombre != 0) {
        nombre /= 10;
        nombre_chiffre_tmp++;
    }
    return nombre_chiffre_tmp;
}

/**
 * \fn void afficherEspaces(int nombre_espaces)
 * \brief Affiche un nombre d espaces
 * \param nombre_espaces Nombre d espaces a afficher
*/
void afficherEspaces(int nombre_espaces) {
    for (int i=0; i < nombre_espaces; i++) {
        printf(" ");
    }
}

/**
 * \fn void afficherLigneSeparatrice(nombre_espaces)
 * \brief Affiche une ligne separatrice
 * \param nombre_espaces Nombre d espaces a afficher
 * 
 * Affiche une ligne separatrice dont la taille est adaptative en fonction de la taille de la grille.
 * Taille minimum de la grille : 1
*/
void afficherLigneSeparatrice(int nombre_espaces) {
    afficherEspaces(nombre_espaces);
    for (int i=0; i < n; i++) {
        printf("+----");
        for (int j=0; j < n-1; j++) {
            printf("---");
        }
    }
    printf("+\n");
}

/**
 * \fn void afficherGrille(candidat grille_candidat)
 * \brief Affiche la grille de jeu de maniere lisible en fonction de TAILLE
 * \param grille_candidat Grille de jeu a afficher
 * 
 * Cette fonction affiche la grille de jeu de maniere lisible en
 * affichant les numeros des lignes et des colonnes, ainsi que les
 * lignes et colonnes de separation entre les blocs de la grille.
 * Les cellules vides sont representees par un point.
 * La taille de la grille est adaptee en fonction de TAILLE.
*/
void afficherGrille(candidat_grille grille_candidat) {
    int i, j, num_espaces; 

    printf("\n");

    // determine le nombre de chiffres dans le nombre le plus grand de la grille
    num_espaces = nombreChiffre(TAILLE) + 1; /** \var nombre d espaces avant le debut de la grille */

    // numeros des colonnes
    afficherEspaces(num_espaces+1);
    for (i = 0; i < TAILLE; i++) {
        // check si i a atteint la fin d une region
        if ((i%n == 0) && (i != 0)){
            printf("  ");
        }
        printf("%3d", i+1);
    }
    printf("\n");

    //affichage de la premiere ligne de separation
    afficherLigneSeparatrice(num_espaces);


    // corps de la grille + cote gauche
    for (i=0; i < TAILLE; i++) {
        //check si i a atteint la fin d une region
        if ((i%n == 0) && (i != 0)){
            afficherLigneSeparatrice(num_espaces);
        }

        // numeros des lignes
        printf("%d", i+1);
        afficherEspaces(num_espaces - nombreChiffre(i+1));
        printf("%c", '|');

        for (j=0; j < TAILLE; j++) {
            // check si j a atteint la fin d une region
            if ((j%n == 0) && (j != 0)){
                printf("%2c",  '|' );
            }

            // affiche '.' au lieu de 0 pour les cellules vides
            if (grille_candidat[i][j].valeur == 0) {
                printf("%3c", CELLULE_VIDE);
            } else {
                printf("%3d", grille_candidat[i][j].valeur);
            }
        }
        printf("%2c",  '|' );
        printf("\n");
    }
    // derniere ligne
    afficherLigneSeparatrice(num_espaces);
}