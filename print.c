#include <stdlib.h>
#include <gtk/gtk.h>
#include "window.h"

extern CallbackWidget widgetCallback;

/*
Démarre l'impression
@params	user_data	Opération d'impression
@params	context	    Paramètre de l'impression
@params	user_data	Paramètre de la fonction de callback
*/
void begin_print(GtkPrintOperation *operation, GtkPrintContext *context, gpointer user_data){
    gtk_print_operation_set_n_pages(operation,1);
}

/*
Création de l'image à imprimer
@params	user_data	Opération d'impression
@params	context	    Paramètre de l'impression
@params	user_data	Paramètre de la fonction de callback
*/
void draw_page(GtkPrintOperation *operation, GtkPrintContext *context, gint page_nr, gpointer user_data){
    GtkWidget *p_grid_box = widgetCallback.entry2; //On récupère le widget de la grille
    cairo_matrix_t mat;
    GdkPixmap* pix = gtk_widget_get_snapshot(p_grid_box, NULL); //On en fait un screenshot
    cairo_t* crp = gtk_print_context_get_cairo_context(context); //On créer l'image

    //Taille impression
    gdouble width = gtk_print_context_get_width(context);
    gdouble height = gtk_print_context_get_height(context);

    //On centre et on redimensionne l'image
    cairo_matrix_init_identity(&mat);
    cairo_matrix_translate(&mat, 0.0, (height-width)/2.0);
    cairo_matrix_scale(&mat, 8.0, 8.0);
    cairo_set_matrix(crp, &mat);

    gdk_cairo_set_source_pixmap(crp, pix, 0, 0);
    cairo_fill(crp);

    //On l'affiche
    cairo_paint(crp) ;
    g_object_unref(pix);
}

/*
Fonction d'impression
@params	p_widget	Widget ayant déclencher l'impression
@params	user_data	Paramètre de la fonction de callback
*/
void cb_print(GtkWidget *p_widget, gpointer user_data){
    GtkPrintOperation *print;
    GtkPrintOperationResult res = GTK_PRINT_OPERATION_RESULT_IN_PROGRESS;

    //On initialise l'impression
    print = gtk_print_operation_new ();
    GtkPrintSettings *settings = gtk_print_settings_new();

    //On paramètre l'impression
    GtkPageSetup *setup = gtk_page_setup_new();
    gtk_print_settings_set_resolution(settings, 100);

    gtk_page_setup_set_orientation(setup, GTK_PAGE_ORIENTATION_PORTRAIT);
    gtk_page_setup_set_top_margin(setup, 15,GTK_UNIT_MM);
    gtk_page_setup_set_left_margin(setup, 15,GTK_UNIT_MM);

    gtk_print_operation_set_default_page_setup(print, setup);
    gtk_print_operation_set_print_settings(print,settings);


    if (settings != NULL){
        //On démarre l'impression
        gtk_print_operation_set_print_settings (print, settings);
        g_signal_connect(print, "begin_print", G_CALLBACK (begin_print), NULL);
        g_signal_connect(print, "draw_page", G_CALLBACK (draw_page), user_data);
        res = gtk_print_operation_run(print, GTK_PRINT_OPERATION_ACTION_PRINT_DIALOG, NULL, NULL);
    }
    if (res == GTK_PRINT_OPERATION_RESULT_APPLY){
        //L'impression a réussi
        if (settings != NULL){
            g_object_unref(settings);
            settings = g_object_ref(gtk_print_operation_get_print_settings (print));
        }
    }
    g_object_unref (print);

    //Parametres inutilises
    (void)p_widget;
    (void)user_data;
}
