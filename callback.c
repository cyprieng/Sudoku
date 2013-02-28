#include <stdlib.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <ctype.h>
#include <math.h>
#include "window.h"
#include "generate.h"
#include "core.h"
#include "file.h"

extern CallbackWidget widgetCallback;

/*
Fermeture de la fenêtre
@params	p_widget	Elément ayant déclencher la fonction
@params	user_data	Données transmis au callback
*/
void cb_quit(GtkWidget *p_widget, gpointer user_data){
    gtk_main_quit();

    //Parametres inutilises
    (void)p_widget;
    (void)user_data;
}

/*
Affichage d'une grille générée
@params	p_widget	Elément ayant déclencher la fonction
@params	user_data	Données transmis au callback
*/
void cb_new_generate(GtkWidget *p_widget, gpointer user_data){
    //Génération
    int grid[9][9] = {{0}};
    generate(grid, (int)user_data);

    int grid_fixes[9][9] = {{0}};
    fill_grid(grid, grid_fixes); //Affichage

    gtk_widget_destroy(p_widget); //Fermeture dialog
}

/*
Nouvelle grille vide ou affichage difficulté
@params	p_widget	Elément ayant déclencher la fonction
@params	user_data	Données transmis au callback
*/
void cb_new(GtkWidget *p_widget, gpointer user_data){
    if((int)user_data == GTK_RESPONSE_ACCEPT){ //Grille vide
        int grid[9][9] = {{0}};
        int grid_fixes[9][9] = {{0}};
        fill_grid(grid, grid_fixes);
    }
    else if((int)user_data == GTK_RESPONSE_REJECT){ //Demande de difficulté
        GtkWidget *p_dialog = gtk_dialog_new_with_buttons(g_locale_to_utf8("Générer grille", -1, NULL, NULL, NULL),
                                                        GTK_WINDOW(widgetCallback.entry1),
                                                        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                                        "Facile",
                                                        EASY_GRID,
                                                        "Moyen",
                                                        MEDIUM_GRID,
                                                        "Difficile",
                                                        HARD_GRID,
                                                        NULL);
        gtk_widget_show(p_dialog);

        g_signal_connect_swapped(p_dialog, "response", G_CALLBACK(cb_new_generate), p_dialog);
    }
    gtk_widget_destroy(p_widget);
}

/*
Affichage boite de dialog "nouveau"
@params	p_widget	Elément ayant déclencher la fonction
@params	user_data	Données transmis au callback
*/
void cb_new_dialog(GtkWidget *p_widget, gpointer user_data){
    GtkWidget *p_dialog = gtk_dialog_new_with_buttons("Nouveau",
                                                        GTK_WINDOW(widgetCallback.entry1),
                                                        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                                        "Rentrer la grille manuellement",
                                                        GTK_RESPONSE_ACCEPT,
                                                        g_locale_to_utf8("Générer automatiquement", -1, NULL, NULL, NULL),
                                                        GTK_RESPONSE_REJECT,
                                                        NULL);
    gtk_widget_show(p_dialog);

    g_signal_connect_swapped(p_dialog, "response", G_CALLBACK(cb_new), p_dialog);

    //Parametres inutilises
    (void)p_widget;
}

/*
Ouverture d'un fichier
@params	p_widget	Elément ayant déclencher la fonction
@params	user_data	Données transmis au callback
*/
void cb_open_file(GtkWidget *p_widget, gpointer user_data){
    GtkWidget *p_dialog_open;
    p_dialog_open = gtk_file_chooser_dialog_new ("Ouvrir un fichier",
                                            GTK_WINDOW(widgetCallback.entry1),
                                            GTK_FILE_CHOOSER_ACTION_OPEN,
                                            GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                            GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                            NULL);

    if (gtk_dialog_run(GTK_DIALOG(p_dialog_open)) == GTK_RESPONSE_ACCEPT){ //Le fichier est choisit
        char *filename;
        filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(p_dialog_open));

        int grid[9][9] = {{0}};
        int grid_fixes[9][9] = {{0}};

        //On récupère et on affiche la grille
        get_grid_from_file(grid, grid_fixes, filename);
        fill_grid(grid, grid_fixes);
    }

    gtk_widget_destroy(p_dialog_open);

    //Parametres inutilises
    (void)p_widget;
}

/*
Sauvegarde d'un grille
@params	p_widget	Elément ayant déclencher la fonction
@params	user_data	Données transmis au callback
*/
void cb_save_file(GtkWidget *p_widget, gpointer user_data){
    GtkWidget *p_dialog_save;
    p_dialog_save = gtk_file_chooser_dialog_new ("Enregistrer un fichier",
                                            GTK_WINDOW(widgetCallback.entry1),
                                            GTK_FILE_CHOOSER_ACTION_SAVE,
                                            GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                            GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
                                            NULL);

    gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(p_dialog_save), TRUE);

   if (gtk_dialog_run(GTK_DIALOG(p_dialog_save)) == GTK_RESPONSE_ACCEPT){ //Nom du fichier a été choisi
        char *filename;
        filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(p_dialog_save));

        int grid[9][9] = {{0}};
        int grid_fixes[9][9] = {{0}};

        //On récupère la grille
        get_grid(grid);
        get_fixed_grid(grid_fixes);

        save_grid_in_file(grid, grid_fixes, filename); //On sauvegarde
    }

    gtk_widget_destroy(p_dialog_save);

    //Parametres inutilises
    (void)p_widget;
}

/*
Evènement lors du changement d'une case de la grille
@params	p_widget	Elément ayant déclencher la fonction
@params	user_data	Données transmis au callback
*/
void cb_entry_modified(GtkWidget *p_widget, gpointer user_data){
    int i,row,col, value, incorrect = 0;
    char entry_value = *gtk_entry_get_text(GTK_ENTRY(p_widget));

    if(!isdigit(entry_value)){ //On empêche l'user de saisir autre chose que des chiffres
        gtk_entry_set_text(GTK_ENTRY(p_widget), "");
    }

    int grid[9][9] = {{0}};
    get_grid(grid);
    int grid_fixes[9][9] = {{0}};
    get_fixed_grid(grid_fixes);

    //On vérifit si grid_fixes est vide
    int emptyGridFixes = 1;
    for(i=0;i<=80;i++){
        row = floor(i/9);
        col = i%9;
        if(grid_fixes[col][row] != 0){emptyGridFixes = 0;break;}
    }

    for(i=0;i<81;i++){
        value = *gtk_entry_get_text(GTK_ENTRY(widgetCallback.entry3[i])) - '0'; //On récupère le char de la case
        if(value == -48) value = 0; //" " => 0

        row = floor(i/9);
        col = i%9;

        grid[col][row] = 0;

        if((!absentDeBloc(value, grid, col, row) || !absentDeColonne(value, grid, row) || !absentDeLigne(value, grid, col)) && grid_fixes[col][row] != 1 && value != 0){
            //La valeur n'est pas valide => on change le style de la case
            GdkColor red;
            gdk_color_parse("#d34836", &red);

            gtk_widget_modify_text(GTK_WIDGET(widgetCallback.entry3[i]), GTK_STATE_NORMAL, &red);
            incorrect++;
        }
        else if(grid_fixes[col][row] == 1 || emptyGridFixes){
            //Base de la grille => style noir
            GdkColor black;
            gdk_color_parse ("#000000", &black);
            gtk_widget_modify_text(GTK_WIDGET(widgetCallback.entry3[i]), GTK_STATE_NORMAL, &black);
        }
        else{
            //Case valide => style bleu
            GdkColor blue;
            gdk_color_parse ("#335D6A", &blue);
            gtk_widget_modify_text(GTK_WIDGET(widgetCallback.entry3[i]), GTK_STATE_NORMAL, &blue);
        }

        grid[col][row] = value;
    }

    if(incorrect == 0){ //Pas d'erreur
        //On vérifit si la grille est remplie
        int filledGrid = 1;
        for(i=0;i<=80;i++){
            row = floor(i/9);
            col = i%9;
            if(grid[col][row] == 0){filledGrid = 0;break;}
        }

        if(filledGrid){ //Rempli + pas d'erreur => grille résolue
            GtkWidget *p_dialog = gtk_message_dialog_new(GTK_WINDOW(widgetCallback.entry1),
                                                        GTK_DIALOG_DESTROY_WITH_PARENT,
                                                        GTK_MESSAGE_INFO,
                                                        GTK_BUTTONS_CLOSE,
                                                        g_locale_to_utf8("Bravo la grille est résolue !", -1, NULL, NULL, NULL),
                                                        NULL);

            gtk_dialog_run (GTK_DIALOG (p_dialog));
            gtk_widget_destroy (p_dialog);
        }
    }

    //Parametres inutilises
    (void)p_widget;
    (void)user_data;
}

/*
Affichage de la solution de la grille
@params	p_widget	Elément ayant déclencher la fonction
@params	user_data	Données transmis au callback
*/
void cb_resolve(GtkWidget *p_widget, gpointer user_data){
    int i,row,col;

    //On récupère la grille
    int grid[9][9] = {{0}};
    get_grid(grid);
    int grid_fixes[9][9] = {{0}};
    get_fixed_grid(grid_fixes);

    //On vérifit si grid_fixes est vide
    int emptyGridFixes = 1;
    for(i=0;i<=80;i++){
        row = floor(i/9);
        col = i%9;
        if(grid_fixes[col][row] != 0){emptyGridFixes = 0;break;}
    }

    if(!emptyGridFixes){
        //On récupère la base de la grille
        for(i=0;i<=80;i++){
            row = floor(i/9);
            col = i%9;
            if(grid_fixes[col][row] == 0){grid[col][row] = 0;}
        }
    }
    else{
        //On remplit grid_fixes(grille saisi par user)
        for(i=0;i<=80;i++){
            row = floor(i/9);
            col = i%9;
            if(grid[col][row] != 0){grid_fixes[col][row] = 1;}
        }
    }

    //On résout et on affiche
    if(is_grid_valid(grid)){
        resolve(grid, 0, 0);
        fill_grid(grid, grid_fixes);
    }

    (void)p_widget;
    (void)user_data;
}
