#ifndef CORE_H_INCLUDED
#define CORE_H_INCLUDED

void fill_grid(int grid[9][9], int grid_fixes[9][9]); //Remplissage de la grille
void get_grid(int grid[9][9]); //Récupération de la grille
void get_fixed_grid(int grid_fixes[9][9]); //Récupération de la grille des elts fixés

int is_grid_valid(int grid[9][9]); //Vérifit si une grille est valide
int equal_grid(int grid1[9][9], int grid2[9][9]); //Vérifit si deux grille sont égales

#endif // CORE_H_INCLUDED
