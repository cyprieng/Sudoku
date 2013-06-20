#ifndef WINDOW_H_INCLUDED
#define WINDOW_H_INCLUDED

//Structure permettant la transmission de widgets aux fonctions de callback
typedef struct{
    GtkWidget *entry1;
    GtkWidget *entry2;
    GtkWidget *entry3[81];
}CallbackWidget;

//Fonctions pour la création du menu
void menu_item_new (GtkMenu *p_menu, const gchar *title, GCallback callback, gpointer user_data);
void menu_separator_new (GtkMenu *p_menu);
GtkMenuBar *menu_new (gpointer user_data);

//Fonctions pour la création de la toolbar
void toolbar_item_new (GtkToolbar *p_toolbar, const gchar *stock_id, const gchar *text, GCallback callback, gpointer user_data);
GtkToolbar *toolbar_new (gpointer user_data);

GtkWidget *grid_new(gpointer user_data); //Création d'une grille

#endif // WINDOW_H_INCLUDED
