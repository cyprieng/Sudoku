#include <stdlib.h>
#include <gtk/gtk.h>
#include "callback.h"
#include "window.h"

CallbackWidget widgetCallback;

int main (int argc, char **argv){
    //Déclaration des widgets
    GtkWidget *p_window = NULL;
    GtkWidget *p_main_box = NULL;
    GtkWidget *p_grid_box = NULL;
    GtkWidget *grid_box_background = NULL;

    //Initialisation de GTK+
    gtk_init (&argc, &argv);

    //Creation de la fenetre principale
    p_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(p_window), 600, 600);
    gtk_window_set_title (GTK_WINDOW (p_window), "Sudoku");
    gtk_window_set_default_icon_from_file("img/icon.png", NULL);
    g_signal_connect (G_OBJECT (p_window), "destroy", G_CALLBACK (cb_quit), NULL);

    //Creation du conteneur principal
    p_main_box = gtk_vbox_new (FALSE, 0);
    gtk_container_add (GTK_CONTAINER (p_window), p_main_box);

    //Widget à transmettre aux fonctions de callback
    widgetCallback.entry1 = p_window;
    widgetCallback.entry2 = NULL;

    //Ajout du menu & toolbar
    gtk_box_pack_start (GTK_BOX (p_main_box), GTK_WIDGET (menu_new (p_main_box)), FALSE, FALSE, 0);
    gtk_box_pack_start (GTK_BOX (p_main_box), GTK_WIDGET (toolbar_new (p_main_box)), FALSE, FALSE, 0);

    //Création de la grille
    p_grid_box = gtk_alignment_new(0.5, 0.5, 0, 0);
    gtk_container_add (GTK_CONTAINER (p_main_box), p_grid_box);
    widgetCallback.entry2 = p_grid_box;

    //On créer le conteneur qui permettra d'avoir un fond à notre tableau
    grid_box_background = gtk_event_box_new();
    gtk_event_box_set_above_child(GTK_EVENT_BOX(grid_box_background), 0);
    gtk_container_add (GTK_CONTAINER (p_grid_box), grid_box_background);

    gtk_container_add(GTK_CONTAINER(grid_box_background), GTK_WIDGET (grid_new (p_grid_box))); //On ajoute les cases

    //Couleur noir
    GdkColor black;
    black.pixel = 32;
    black.red = 0;
    black.green = 0;
    black.blue = 0;

    gtk_widget_modify_bg(GTK_WIDGET(grid_box_background), GTK_STATE_NORMAL, &black); //On colore le fond en noir

    //Affichage de la fenetre principale
    gtk_widget_show_all (p_window);

    //Lancement de la boucle principale
    gtk_main ();

    return EXIT_SUCCESS;
}
