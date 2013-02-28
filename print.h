#ifndef PRINT_H_INCLUDED
#define PRINT_H_INCLUDED

//Fonction d'impression
void begin_print(GtkPrintOperation *operation, GtkPrintContext *context, gpointer user_data);
void draw_page(GtkPrintOperation *operation, GtkPrintContext *context, gint page_nr, gpointer user_data);
void cb_print (GtkWidget *p_widget, gpointer user_data);

#endif // PRINT_H_INCLUDED
