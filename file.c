#include <stdlib.h>
#include <stdio.h>

/*
Récupère la grille à partir d'un fichier
@params	grid	        Grille à remplir
@params	grid_fixes	    Grille elt fixes à remplir
@params	filename	    Nom du fichier
*/
void get_grid_from_file(int grid[][9], int grid_fixes[][9], char *filename){
    FILE *ptr = NULL;
    char char_inter;
    int ligne, colonne;

    ptr = fopen(filename, "r");
    if(ptr == NULL) return; //On test l'ouverture du fichier

    for(ligne=0; ligne<9; ligne++)
    {
        for(colonne=0; colonne<9; colonne++)
        {
            fscanf(ptr, "%1[0-9]", &char_inter); //On récupère les élts des grilles
            grid[ligne][colonne] = char_inter - '0'; //On convertir le char en int

            fscanf(ptr, "%1[0-9] ", &char_inter);
            grid_fixes[ligne][colonne] = char_inter - '0';
        }
    }

    fclose(ptr);
}

/*
Sauvegarde la gille dans un fichier
@params	grid	        Grille à enregistrer
@params	grid_fixes	    Grille elt fixes à enregistrer
@params	filename	    Nom du fichier
*/
void save_grid_in_file(int grid[][9], int grid_fixes[][9], char *filename){
    FILE * ptr = NULL;
    int ligne, colonne;

    ptr = fopen(filename, "w");
    if(ptr == NULL) return; //On test l'ouverture du fichier

    for(ligne=0; ligne<9; ligne++)
    {
        for(colonne=0; colonne<9; colonne++)
        {
            //On insère le contenu des grilles
            fprintf(ptr, "%d", grid[ligne][colonne]);
            fprintf(ptr, "%d ", grid_fixes[ligne][colonne]);
        }
    }
    fclose(ptr);
}
