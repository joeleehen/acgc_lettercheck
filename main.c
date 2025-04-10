#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include "trigram_table.h"
#include "ascii.h"
#include "lettercheck.h"

GtkWidget *txt;
letter_scores scores;
letter_scores *score_ptr = &scores;

void end_program(GtkWidget *wid, gpointer ptr) {
    gtk_main_quit();
}

void get_letter_score(GtkWidget *wid, gpointer ptr) {
    char buffer[20];
    char *letter = gtk_entry_get_text(GTK_ENTRY(txt));
    int letter_score = score_letter(letter, strlen(letter), score_ptr);
    sprintf(buffer, "Letter score: %d", score_ptr->total_score);
    gtk_label_set_text(GTK_LABEL(ptr), buffer);
}

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);
    GtkWidget *win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(win, "delete_event", G_CALLBACK(end_program), NULL);

    GtkWidget *box = gtk_vbox_new(FALSE, 10);
    GtkWidget *title_label = gtk_label_new("ACGC Letter Grader");
    txt = gtk_entry_new();
    GtkWidget *score_button = gtk_button_new_with_label("Score Letter");
    GtkWidget *score_label = gtk_label_new("Letter score:");
    g_signal_connect(score_button, "clicked", G_CALLBACK(get_letter_score), score_label);
    g_signal_connect(txt, "activate", G_CALLBACK(get_letter_score), score_label);

    gtk_box_pack_start(GTK_BOX(box), title_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), txt, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), score_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), score_label, FALSE, FALSE, 0);
    gtk_container_add(GTK_CONTAINER(win), box);

    gtk_widget_show_all(win);
    gtk_main();
    return 0;
}
