#ifndef CALLBACK_H_INCLUDED
#define CALLBACK_H_INCLUDED

void cb_quit (GtkWidget *p_widget, gpointer user_data); //Callback qui quitte notre application
void cb_new_dialog(GtkWidget *p_widget, gpointer user_data); //Nouveau fichier
void cb_new_generate(GtkWidget *p_widget, gpointer user_data); //Nouvelle grille générée

void cb_open_file(GtkWidget *p_widget, gpointer user_data); //Chargement
void cb_save_file(GtkWidget *p_widget, gpointer user_data); //Enregistrement

void cb_entry_modified(GtkWidget *p_widget, gpointer user_data); //Champ texte modifié

void cb_resolve(GtkWidget *p_widget, gpointer user_data); //Résolution de la grille

#endif // CALLBACK_H_INCLUDED
