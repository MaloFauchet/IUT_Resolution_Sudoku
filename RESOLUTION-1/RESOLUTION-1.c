/**
 * \file BACKTRACKING-1.c
 * \brief Programme de resolution de sudoku
 * \author PFRANGER Mathéo, FAUCHET Malo
 * \date 13 Janvier 2024
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


// tGrille est un tableau de TAILLE lignes et TAILLE colonnes contenant la grille de jeu initiale
typedef int tGrille[TAILLE][TAILLE];


/**
 * \def CELLULE_VIDE
 * \brief Caractere representant une cellule vide
*/
const char CELLULE_VIDE = '.' ;


bool absentSurLigne(int valeur, tGrille grille, int numLigne);
bool absentSurColonne(int valeur, tGrille grille, int numColonne);
bool absentSurBloc(int valeur, tGrille grille, int numLigne, int numColonne);

bool backtracking(tGrille grille, int numCase);

int premiereCaseVide(tGrille grille);

char chargerGrille(tGrille grille);
int nombreChiffre(int nombre);
void afficherEspaces(int nombre_espaces);
void afficherLigneSeparatrice(int nombre_espaces);
void afficherGrille(tGrille grille);


/*****************************************************
 *                PROGRAMME PRINCIPAL                *
 *****************************************************/

int main() {
    tGrille grille;

    chargerGrille(grille);

    printf("Grille initial\n");
    afficherGrille(grille);

    clock_t start = clock();

    backtracking(grille, premiereCaseVide(grille));

    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    
    printf("Grille finale\n");
    afficherGrille(grille);
    printf("Resolution de la grille en %.6f secondes\n", time_spent);

    return EXIT_SUCCESS;
}

/**
 * \fn bool absentSurLigne(int valeur, tGrille grille, int numLigne
 * \brief Verifie si une valeur est absente sur une ligne
 * \param valeur Valeur dont on veut verifier l absence sur la ligne
 * \param grille Grille de jeu
 * \param numLigne Numero de la ligne de la case selectionnee
 * 
 * \return true si la valeur est absente sur la ligne, false sinon
*/
bool absentSurLigne(int valeur, tGrille grille, int numLigne) {
    int i;
    bool absent = true;
    i = 0;
    while (i < TAILLE && absent) {
        if (grille[numLigne][i] == valeur) {
            absent = false;
        }
        i++;
    }
    return absent;
}

/**
 * \fn bool absentSurColonne(int valeur, tGrille grille, int numColonne)
 * \brief Verifie si une valeur est absente sur une colonne
 * \param valeur Valeur dont on veut verifier l absence sur la colonne
 * \param grille Grille de jeu
 * \param numColonne Numero de la colonne de la case selectionnee
 * 
 * \return true si la valeur est absente sur la colonne, false sinon
*/
bool absentSurColonne(int valeur, tGrille grille, int numColonne) {
    int i;
    bool absent = true;
    i = 0;
    while (i < TAILLE && absent) {
        if (grille[i][numColonne] == valeur) {
            absent = false;
        }
        i++;
    }
    return absent;
}

/**
 * \fn bool absentSurBloc(int valeur, tGrille grille, int numLigne, int numColonne)
 * \brief Verifie si une valeur est absente sur un bloc
 * \param valeur Valeur dont on veut verifier l absence sur le bloc
 * \param grille Grille de jeu
 * \param numLigne Numero de la ligne de la case selectionnee
 * \param numColonne Numero de la colonne de la case selectionnee
 * 
 * \return true si la valeur est absente sur le bloc, false sinon
*/
bool absentSurBloc(int valeur, tGrille grille, int numLigne, int numColonne) {
    int startingRow = numLigne - (numLigne%n);
    int startingCol = numColonne - (numColonne%n);

    int i, j;
    bool absent = true;
    i = startingRow;
    while (i < startingRow + n && absent) {
        j = startingCol;
        while (j < startingCol + n && absent) {
            if (grille[i][j] == valeur) {
                absent = false;
            }
            j++;
        }
        i++;
    }
    return absent;
}

/**
 * \fn bool backtracking(tGrille grille, int numCase)
 * \brief Resout la grille de jeu par backtracking
 * \param grille Grille de jeu
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
bool backtracking(tGrille grille, int numCase){
    int lig, col;
    bool res = false;

    if(numCase == TAILLE*TAILLE){
        // On a traité tout les cases, la grille est résolue
        res = true;
    } else {
        // On récupère les "coordonnées" de la case
        lig = numCase / TAILLE;
        col = numCase % TAILLE;
        if (grille[lig][col] != 0) {
            // La case n'est pas vide, on passe à la suivante
            // (appel récursif)
            res = backtracking(grille, numCase+1);
        } else {
            for (int val = 1; val <= TAILLE; val++) {
                if (absentSurLigne(val, grille, lig) 
                && absentSurColonne(val, grille, col) 
                && absentSurBloc(val, grille, lig, col)) {
                    // Si la valeur est autorisée, on l'inscrit dans la case
                    grille[lig][col] = val;
                    // Et on passe à la case suivante : appel récursif
                    // pour voir si ce choix est bon par la suite
                    if (backtracking(grille, numCase+1) == true) {
                        res = true;
                    } else {
                        grille[lig][col] = 0;
                    }
                }
            }
        } 
    }
    return res;
}

/**
 * \fn int premiereCaseVide(tGrille grille)
 * \brief Determine la premiere case vide de la grille
 * \param grille Grille de jeu
 * 
 * \return Numero de la premiere case vide de la grille
 * 
 * Cette fonction determine la premiere case vide de la grille
 * en parcourant la grille de gauche a droite et de haut en bas.
 * Si aucune case vide n est trouvee, la fonction retourne -1.
*/
int premiereCaseVide(tGrille grille) {
    int i, j;
    bool trouve = false;
    i = 0;
    while (i < TAILLE && !trouve) {
        j = 0;
        while (j < TAILLE && !trouve) {
            if (grille[i][j] == 0) {
                trouve = true;
            }
            j++;
        }
        i++;
    }
    return (i-1)*TAILLE + (j-1);
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
    char nomFichier[30];  /** \var nomFichier chaine de caracteres contenant le nom du fichier de grille a initialiser */
    char numFic;  /** \var numFic le numero du fichier de grille a initialiser */

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
 * \fn void afficherGrille(tGrille grille)
 * \brief Affiche la grille de jeu de maniere lisible en fonction de TAILLE
 * \param grille Grille de jeu a afficher
 * 
 * Cette fonction affiche la grille de jeu de maniere lisible en
 * affichant les numeros des lignes et des colonnes, ainsi que les
 * lignes et colonnes de separation entre les blocs de la grille.
 * Les cellules vides sont representees par un point.
 * La taille de la grille est adaptee en fonction de TAILLE.
*/
void afficherGrille(tGrille grille) {
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
            if (grille[i][j] == 0) {
                printf("%3c", CELLULE_VIDE);
            } else {
                printf("%3d", grille[i][j]);
            }
        }
        printf("%2c",  '|' );
        printf("\n");
    }
    // derniere ligne
    afficherLigneSeparatrice(num_espaces);
}