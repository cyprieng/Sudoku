#include <stdlib.h>
#include <gtk/gtk.h>
#include <math.h>
#include "window.h"
#include "callback.h"
#include "generate.h"

extern CallbackWidget widgetCallback;

/*
Remplit la grille
@params	grid	    Grille à afficher
@params	grid_fixes	Grille des éléments fixés
*/
void fill_grid(int grid[9][9], int grid_fixes[9][9]){
    int i = 0;
    char number;
    int row,col;
    int condition;

    //Style bleu => elt saisi par l'utilisateur
    GdkColor blue;
    gdk_color_parse ("#335D6A", &blue);

    //Style noir => elt de la base de la grille
    GdkColor black;
    gdk_color_parse ("#000000", &black);

    //Police de base
    PangoFontDescription *font;
    font =  pango_font_description_from_string ("Sans bold 30");
    pango_font_description_set_style (font, PANGO_STYLE_ITALIC);

    //On vérifit si la grille est vide
    int emptyGrid = 1;
    for(i=0;i<=80;i++){
        row = floor(i/9);
        col = i%9;
        if(grid[col][row] != 0){emptyGrid = 0;break;}
    }

    //On vérifit si grid_fixes est vide
    int emptyGridFixes = 1;
    for(i=0;i<=80;i++){
        row = floor(i/9);
        col = i%9;
        if(grid_fixes[col][row] != 0){emptyGridFixes = 0;break;}
    }

    for(i=0;i<=80;i++){
        row = floor(i/9);
        col = i%9;
        number = (char)(((int)'0')+grid[col][row]);

        g_signal_handlers_destroy(GTK_OBJECT(widgetCallback.entry3[i])); //On empêche gtk d'afficher l'evt "grille résolue"

        //Condition : case ne faisant pas partie de la base
        if(!emptyGridFixes){condition = (number == '0' || grid_fixes[col][row] == 0);}
        else{condition = (number == '0');}

        if(condition){
            if(number == '0'){ //0 => case vide
                gtk_entry_set_text(GTK_ENTRY(widgetCallback.entry3[i]), "");
            }
            else{
                gtk_entry_set_text(GTK_ENTRY(widgetCallback.entry3[i]), &number);
            }

            if(!emptyGrid){ //Si non vide => style bleu
                gtk_widget_modify_font(GTK_WIDGET(widgetCallback.entry3[i]), font);
                gtk_widget_modify_text(GTK_WIDGET(widgetCallback.entry3[i]), GTK_STATE_NORMAL, &blue);
            }
            else{ //Sinon base => style noir
                gtk_widget_modify_font(GTK_WIDGET(widgetCallback.entry3[i]), pango_font_description_from_string ("Sans bold 30"));
                gtk_widget_modify_text(GTK_WIDGET(widgetCallback.entry3[i]), GTK_STATE_NORMAL, &black);
            }
            gtk_editable_set_editable(GTK_EDITABLE(widgetCallback.entry3[i]), 1);
        }
        else{ //Sinon base => style noir
            gtk_entry_set_text(GTK_ENTRY(widgetCallback.entry3[i]), &number);
            gtk_widget_modify_font(GTK_WIDGET(widgetCallback.entry3[i]), pango_font_description_from_string ("Sans bold 30"));
            gtk_widget_modify_text(GTK_WIDGET(widgetCallback.entry3[i]), GTK_STATE_NORMAL, &black);
            gtk_editable_set_editable(GTK_EDITABLE(widgetCallback.entry3[i]), 0);
        }

        g_signal_connect (GTK_EDITABLE(widgetCallback.entry3[i]), "changed", G_CALLBACK(cb_entry_modified), NULL);
    }
}

/*
Récupère la grille
@params	grid	   Grille actuel à remplir
*/
void get_grid(int grid[9][9]){
    int i = 0;
    int row,col;

     for(i=0;i<=80;i++){
        row = floor(i/9);
        col = i%9;
        grid[col][row] = *gtk_entry_get_text(GTK_ENTRY(widgetCallback.entry3[i])) - '0';
        if(grid[col][row] == -48){ //" " => 0
            grid[col][row] = 0;
        }
    }
}

/*
Récupère la grille des elts fixés
@params	grid_fixes	   Grille à remplir
*/
void get_fixed_grid(int grid_fixes[9][9]){
    int i = 0;
    int row,col;

     for(i=0;i<=80;i++){
        row = floor(i/9);
        col = i%9;

        //Si le GTK_ENTRY n'est pas éditable => case fixe
        if(gtk_editable_get_editable(GTK_EDITABLE(widgetCallback.entry3[i]))){
            grid_fixes[col][row] = 0;
        }
        else{
            grid_fixes[col][row] = 1;
        }
    }
}

/*
Test si une grille est valide
@params	grid	   Grille à tester
@return 1 si valide, 0 sinon
*/
int is_grid_valid(int grid[9][9]){
    int i = 0;
    int row,col;
    int value;

     for(i=0;i<=80;i++){
        row = floor(i/9);
        col = i%9;
        value = grid[col][row];

        grid[col][row] = 0;

        if((!absentDeBloc(value, grid, col, row) || !absentDeColonne(value, grid, row) || !absentDeLigne(value, grid, col)) && value != 0){
            //Valeur non valide
            grid[col][row] = value;
            return 0;
        }

        grid[col][row] = value;
    }

    return 1;
}

/*
Vérifit si deux grilles sont identiques
@params grid1   Première grille à comparer
@params grid2   Deuxième grille à comparer
@return 1 si égales, 0 sinon
*/
int equal_grid(int grid1[9][9], int grid2[9][9]){
    int i = 0;
    int row,col;

     for(i=0;i<=80;i++){
        row = floor(i/9);
        col = i%9;

        if(grid1[col][row] != grid2[col][row]){
            //Grilles différentes
            return 0;
        }
    }

    return 1;
}
