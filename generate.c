#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "core.h"

/*
Vérifit si valeur absent d'une ligne de la grille
@params	k	        Valeur à tester
@params	grille	    Grille
@params	i	        ligne
@return 1 si absent, 0 sinon
*/
int absentDeLigne(int k, int grille[9][9], int i){
    int j;

    for(j=0; j < 9; j++){
        if(grille[i][j] == k) return 0;
    }

    return 1;
}

/*
Vérifit si valeur absent d'une colonne de la grille
@params	k	        Valeur à tester
@params	grille	    Grille
@params	j	        colonne
@return 1 si absent, 0 sinon
*/
int absentDeColonne(int k, int grille[9][9], int j){
    int i;

    for (i=0; i < 9; i++){
        if(grille[i][j] == k) return 0;
    }

    return 1;
}

/*
Vérifit si valeur absent d'un bloc de la grille
@params	k	        Valeur à tester
@params	grille	    Grille
@params	i	        ligne
@params	j	        colonne
@return 1 si absent, 0 sinon
*/
int absentDeBloc (int k, int grille[9][9], int i, int j){
    int _i = i-(i%3), _j = j-(j%3);

    //On boucle le carré
    for (i=_i; i < _i+3; i++){
        for (j=_j; j < _j+3; j++){
            if (grille[i][j] == k) return 0;
        }
    }

    return 1;
}

/*
Résout ou génère une grille
@params	grille      Grille à résoudre
@params	position    Position en cours de test
@params	generate    ndique si génération ou non
@return 1 si aucune erreur, 0 sinon
*/
int resolve(int grille[9][9], int position, int generate){
    int k;
    if(position == 9*9) return 1; //Grille résolue

    int i = position/9, j = position%9;

    //Case déjà cimpléter
    if(grille[i][j] != 0)
        return resolve(grille, position+1, generate);

    if(!generate){
        for(k=1; k <= 9; k++){
            //On test la valeur k
            if(absentDeLigne(k,grille,i) && absentDeColonne(k,grille,j) && absentDeBloc(k,grille,i,j)){
                grille[i][j] = k;

                if(resolve(grille, position+1, generate)) return 1;
            }
        }
    }
    else{ //Génération
        int nb;
        srand(time(NULL));

        for(k=1; k <= 20; k++){
            nb = rand()%9 + 1; //On test des nombres aléatoires
            if (absentDeLigne(nb,grille,i) && absentDeColonne(nb,grille,j) && absentDeBloc(nb,grille,i,j)){
                grille[i][j] = nb;

                if(resolve(grille, position+1, generate)) return 1;
            }
        }
    }
    grille[i][j] = 0;

    return 0; //Grille non résoluble
}

/*
Test si une grille a une seule solution
@params grid    grille à tester
@return 1 si unique, 0 sinon
*/
int isUnique(int grid[][9]){
    int grid_copy[9][9] = {{0}}; //Copie de grid
    int grid_to_resolve[9][9] = {{0}}; //Gille à résoudre
    int grid_resolve[9][9] = {{0}}; //Grille contenant la résolution
    int grid_fixes[9][9] = {{0}}; //Grille des elts fixes

    int i = 0, k = 0;
    int row,col;
    int value;

    //On génère la grille des elts fixés
    for(i=0;i<=80;i++){
        row = floor(i/9);
        col = i%9;
        if(grid[col][row] != 0){grid_fixes[col][row] = 1;}
    }

    //On copie la grille
    memcpy(grid_to_resolve, grid, sizeof(grid_to_resolve));
    memcpy(grid_copy, grid, sizeof(grid_copy));

    resolve(grid_copy, 0, 0); //On résoud grid_copy

     for(i=0;i<=80;i++){
        row = floor(i/9);
        col = i%9;

        if(grid_fixes[col][row] == 1)continue; //Elt fixe => pas de test

        value = grid_to_resolve[col][row];

        //On test de résoudre la grille avec chaque k, pour voir si il y a deux solutions
        for(k=1;k<=9;k++){
            grid_to_resolve[col][row] = k;

            if(!is_grid_valid(grid_to_resolve)) continue; //Grille pas valide => pas de solution

            memcpy(grid_resolve, grid_to_resolve, sizeof(grid_resolve));

            if(!resolve(grid_resolve, 0, 0)) continue; //Pas de solution

            if(!equal_grid(grid_copy, grid_resolve))return 0; //On a trouvé une solution différente de grid_copy => pas unique
        }

        grid_to_resolve[col][row] = value;
    }

	return 1;
}

/*
Génère une grille
@params	grid	    Grille renvoyée
@params	type	    Difficulté de la grille
*/
void generate(int grid[][9], int type){
    int i, valide, position, row, col, inter;

    resolve(grid, 0, 1);

    //On supprime des cases aléatoirement en vérifiant l'unicité
    srand(time(NULL));
    for(i=0;i<type;i++){
        valide = 0;

        do{
            position = rand() % 81;

            row = floor(position/9);
            col = position%9;

            if(grid[col][row] == 0) continue; //Case déjà supprimée

            inter = grid[col][row];

            grid[col][row] = 0;

            //On vérifit si la grille est toujours unique
            if(isUnique(grid)){valide = 1;}
            else{grid[col][row] = inter;}
        }while(!valide);
    }
}
