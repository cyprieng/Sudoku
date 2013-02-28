#include <stdlib.h>
#include <gtk/gtk.h>
#include <math.h>
#include "callback.h"
#include "print.h"

//Structure permettant la transmission de widgets aux fonctions de callback
typedef struct{
    GtkWidget *entry1;
    GtkWidget *entry2;
    GtkWidget *entry3[81];
}CallbackWidget;
extern CallbackWidget widgetCallback;

/*
Ajoute des item à un menu
@params	p_menu		Menu où l'on veut ajouter des items
@params	title	    Nom de l'item
@params	callback    Fonction de callback de l'item
@params	user_data   Données à transmettre à la fonction de callback
*/
static void menu_item_new (GtkMenu *p_menu, const gchar *title, GCallback callback, gpointer user_data){
    GtkWidget *p_menu_item = NULL;

    p_menu_item = gtk_menu_item_new_with_mnemonic (title);
    gtk_menu_shell_append (GTK_MENU_SHELL (p_menu), p_menu_item);
    g_signal_connect (G_OBJECT (p_menu_item), "activate", callback, user_data);
}

/*
Ajoute un séparateur à un menu
@params	p_menu		Menu où l'on veut ajouter le separateur
*/
static void menu_separator_new (GtkMenu *p_menu){
    GtkWidget *p_menu_item = NULL;

    p_menu_item = gtk_separator_menu_item_new();
    gtk_menu_shell_append (GTK_MENU_SHELL (p_menu), p_menu_item);
}

/*
Créer le menu
@params	user_data	Paramètre de la fonction de callback
@return p_menu_bar contenant le menu
*/
GtkMenuBar *menu_new (gpointer user_data){
    //On créer la barre de menu
    GtkWidget *p_menu_bar = NULL;
    p_menu_bar = gtk_menu_bar_new ();

    //Menu "fichier"
    GtkWidget *p_menu = NULL;
    GtkWidget *p_menu_item = NULL;

    p_menu = gtk_menu_new ();
    p_menu_item = gtk_menu_item_new_with_mnemonic("_Fichier");

    //On ajoute les items
    menu_item_new (GTK_MENU (p_menu), "_Nouveau", G_CALLBACK (cb_new_dialog), user_data);
    menu_separator_new(GTK_MENU (p_menu));
    menu_item_new (GTK_MENU (p_menu), "_Ouvrir", G_CALLBACK (cb_open_file), user_data);
    menu_item_new (GTK_MENU (p_menu), "_Enregistrer", G_CALLBACK (cb_save_file), user_data);
    menu_separator_new(GTK_MENU (p_menu));
    menu_item_new (GTK_MENU (p_menu), g_locale_to_utf8("_Résoudre", -1, NULL, NULL, NULL), G_CALLBACK (cb_resolve), user_data);
    menu_separator_new(GTK_MENU (p_menu));
    menu_item_new (GTK_MENU (p_menu), "_Imprimer", G_CALLBACK (cb_print), user_data);
    menu_separator_new(GTK_MENU (p_menu));
    menu_item_new (GTK_MENU (p_menu), "_Quitter", G_CALLBACK (cb_quit), user_data);

    gtk_menu_item_set_submenu (GTK_MENU_ITEM (p_menu_item), p_menu);
    gtk_menu_shell_append (GTK_MENU_SHELL (p_menu_bar), p_menu_item);

    return GTK_MENU_BAR (p_menu_bar);
}


/*
Ajoute des item à une toolbar
@params	p_toolbar	Toolbar où l'on veut ajouter des items
@params	stock_id    Id du bouton à ajouter
@params	callback    Fonction de callback de l'item
@params	user_data   Données à transmettre à la fonction de callback
*/
static void toolbar_item_new (GtkToolbar *p_toolbar, const gchar *stock_id, const gchar *text, GCallback callback, gpointer user_data){
    GtkToolItem *p_tool_item = NULL;

    p_tool_item = gtk_tool_button_new_from_stock (stock_id);
    gtk_tool_item_set_tooltip_text(p_tool_item, text);
    g_signal_connect (G_OBJECT (p_tool_item), "clicked", callback, user_data);
    gtk_toolbar_insert (p_toolbar, p_tool_item, -1);
}

/*
Créer la toolbar
@params	user_data	Paramètre de la fonction de callback
@return p_toolbar contenant la toolbar
*/
GtkToolbar *toolbar_new (gpointer user_data){
    //On créer la toolbar
    GtkWidget *p_toolbar = NULL;
    p_toolbar = gtk_toolbar_new ();

    //On ajoute les items
    toolbar_item_new (GTK_TOOLBAR (p_toolbar), GTK_STOCK_NEW, "Nouveau", G_CALLBACK (cb_new_dialog), user_data);
    toolbar_item_new (GTK_TOOLBAR (p_toolbar), GTK_STOCK_OPEN, "Ouvrir", G_CALLBACK (cb_open_file), user_data);
    toolbar_item_new (GTK_TOOLBAR (p_toolbar), GTK_STOCK_SAVE, "Enregistrer", G_CALLBACK (cb_save_file), user_data);
    toolbar_item_new (GTK_TOOLBAR (p_toolbar), GTK_STOCK_EXECUTE, g_locale_to_utf8("Résoudre", -1, NULL, NULL, NULL), G_CALLBACK (cb_resolve), user_data);
    toolbar_item_new (GTK_TOOLBAR (p_toolbar), GTK_STOCK_PRINT, "Imprimer", G_CALLBACK (cb_print), user_data);
    toolbar_item_new (GTK_TOOLBAR (p_toolbar), GTK_STOCK_QUIT, "Quitter", G_CALLBACK (cb_quit), user_data);

    //On supprime le nom des boutons
    gtk_toolbar_set_style (GTK_TOOLBAR (p_toolbar), GTK_TOOLBAR_ICONS);

    return GTK_TOOLBAR (p_toolbar);
}


/*
Créer la grille de sudoku
@params	user_data	Paramètre de la fonction de callback
@return p_grid contenant le tableau
*/
GtkWidget *grid_new(gpointer user_data){
    GtkWidget *p_grid = NULL;
    GtkWidget *p_text_box[81];
    int i, row = 0, col = 0;

    //Bordure des champs textes
    GtkBorder border;
    border.bottom=0;
    border.top=0;
    border.left=10;
    border.right=0;

    //On créer le tableau
    p_grid = gtk_table_new(9, 9, 1);

    //Espacemens des cases pour gérer la taille de la bordure
    gtk_table_set_row_spacings(GTK_TABLE(p_grid), 1);
    gtk_table_set_col_spacings(GTK_TABLE(p_grid), 1);

    gtk_container_set_border_width(GTK_CONTAINER(p_grid), 5);
    gtk_table_set_col_spacing(GTK_TABLE(p_grid), 2, 5);
    gtk_table_set_col_spacing(GTK_TABLE(p_grid), 5, 5);

    gtk_table_set_row_spacing(GTK_TABLE(p_grid), 2, 5);
    gtk_table_set_row_spacing(GTK_TABLE(p_grid), 5, 5);

    for(i=0;i<=80;i++){ //On boucle les 81 cases
        //Récupération colonne et ligne actuelles
        row = floor(i/9);
        col = i%9;

        //Création champ texte
        p_text_box[i] = gtk_entry_new();
        widgetCallback.entry3[i] = p_text_box[i];
        gtk_entry_set_width_chars(GTK_ENTRY(p_text_box[i]), 1);
        gtk_entry_set_max_length(GTK_ENTRY(p_text_box[i]), 1);
        gtk_widget_set_size_request(GTK_WIDGET(p_text_box[i]), 50, 50);
        gtk_widget_modify_font(GTK_WIDGET(p_text_box[i]), pango_font_description_from_string ("Sans bold 30"));
        g_object_set(G_OBJECT(p_text_box[i]), "inner-border", &border, "shadow-type", GTK_SHADOW_NONE, NULL);
        g_signal_connect(GTK_EDITABLE(p_text_box[i]), "changed", G_CALLBACK(cb_entry_modified), user_data);

        //Ajout champ texte à la table
        gtk_table_attach(GTK_TABLE(p_grid), p_text_box[i],
                            row, row+1,
                            col, col+1,
                            GTK_EXPAND, GTK_EXPAND,
                            0, 0);
    }
    return p_grid;
}
