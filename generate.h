#ifndef GENERATE_H_INCLUDED
#define GENERATE_H_INCLUDED
#define EASY_GRID 40
#define MEDIUM_GRID 45
#define HARD_GRID 55

void generate(int grid[][9], int type); //Génération de la grille

//Test de la grille
int absentDeLigne (int k, int grille[9][9], int i);
int absentDeColonne (int k, int grille[9][9], int j);
int absentDeBloc (int k, int grille[9][9], int i, int j);
int isUnique(int grid[][9]);

int resolve(int grille[9][9], int position, int generate); //Résolution

#endif // GENERATE_H_INCLUDED
