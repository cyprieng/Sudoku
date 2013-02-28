#ifndef FILE_H_INCLUDED
#define FILE_H_INCLUDED

//Fonction de gestion des fichiers
void get_grid_from_file(int grid[][9], int grid_fixes[][9], char *filename);
void save_grid_in_file(int grid[][9], int grid_fixes[][9], char *filename);

#endif // FILE_H_INCLUDED
