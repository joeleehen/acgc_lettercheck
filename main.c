#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include "trigram_table.h"
#include "ascii.h"
#include "lettercheck.h"

GtkWidget *txt;
GtkWidget *check_a_label;
GtkWidget *check_b_label;
GtkWidget *check_c_label;
GtkWidget *check_d_label;
GtkWidget *check_e_label;
GtkWidget *check_f_label;
GtkWidget *check_g_label;

letter_scores scores;
letter_scores *score_ptr = &scores;

void end_program(GtkWidget *wid, gpointer ptr) {
    gtk_main_quit();
}

// NOTE: this is incredibly stupid implementation
void update_check_labels(int check_score, GtkWidget *lbl, char check_char) {
    char buffer[25];
    sprintf(buffer, "Check %c: %d points", check_char, check_score);
    gtk_label_set_text(GTK_LABEL(lbl), buffer);
}

void get_letter_score(GtkWidget *wid, gpointer ptr) {
    char buffer[20];
    char *letter = gtk_entry_get_text(GTK_ENTRY(txt));
    int letter_score = score_letter(letter, strlen(letter), score_ptr);
    sprintf(buffer, "Letter score: %d", score_ptr->total_score);
    gtk_label_set_text(GTK_LABEL(ptr), buffer);

    update_check_labels(score_ptr->checkA, check_a_label, 'A');
    update_check_labels(score_ptr->checkB, check_b_label, 'B');
    update_check_labels(score_ptr->checkC, check_c_label, 'C');
    update_check_labels(score_ptr->checkD, check_d_label, 'D');
    update_check_labels(score_ptr->checkE, check_e_label, 'E');
    update_check_labels(score_ptr->checkF, check_f_label, 'F');
    update_check_labels(score_ptr->checkG, check_g_label, 'G');
}

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);
    GtkWidget *win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(win), 600, 700);
    g_signal_connect(win, "delete_event", G_CALLBACK(end_program), NULL);

    GtkWidget *box = gtk_vbox_new(FALSE, 10);
    GtkWidget *title_label = gtk_label_new("ACGC Letter Grader");
    txt = gtk_entry_new();
    GtkWidget *score_button = gtk_button_new_with_label("Score Letter");
    GtkWidget *score_label = gtk_label_new("Letter score:");

    check_a_label = gtk_label_new("Check A: 0 points");
    check_b_label = gtk_label_new("Check B: 0 points");
    check_c_label = gtk_label_new("Check C: 0 points");
    check_d_label = gtk_label_new("Check D: 0 points");
    check_e_label = gtk_label_new("Check E: 0 points");
    check_f_label = gtk_label_new("Check F: 0 points");
    check_g_label = gtk_label_new("Check G: 0 points");
    GtkWidget *checkbox = gtk_hbox_new(FALSE, 20);

    gtk_box_pack_start(GTK_BOX(checkbox), check_a_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(checkbox), check_b_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(checkbox), check_c_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(checkbox), check_d_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(checkbox), check_e_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(checkbox), check_f_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(checkbox), check_g_label, FALSE, FALSE, 0);

    g_signal_connect(score_button, "clicked", G_CALLBACK(get_letter_score), score_label);
    g_signal_connect(txt, "activate", G_CALLBACK(get_letter_score), score_label);
    // g_signal_connect(txt, "key-press-event", G_CALLBACK(get_letter_score), score_label);

    gtk_box_pack_start(GTK_BOX(box), title_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), txt, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), score_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), score_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), checkbox, FALSE, FALSE, 0);
    gtk_container_add(GTK_CONTAINER(win), box);

    gtk_widget_show_all(win);
    gtk_main();
    return 0;
}
