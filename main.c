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

void update_check_label(int check_score, GtkWidget *lbl) {
    char buffer[15];
    char *markup_formatted;

    sprintf(buffer, "%d points", check_score);

    if (check_score > 0 ) {
        char *format = "<span color=\"green\">\%s</span>";
        markup_formatted = g_markup_printf_escaped(format, buffer);
    } else if (check_score < 0) {
        char *format = "<span color=\"red\">\%s</span>";
        markup_formatted = g_markup_printf_escaped(format, buffer);
    } else {
        char *format = "<span>\%s</span>";
        markup_formatted = g_markup_printf_escaped(format, buffer);
    }

    gtk_label_set_markup(GTK_LABEL(lbl), markup_formatted);
    g_free(markup_formatted);
}

void get_letter_score(GtkWidget *wid, gpointer ptr) {
    char buffer[20];
    char *letter = gtk_entry_get_text(GTK_ENTRY(txt));
    int letter_score = score_letter(letter, strlen(letter), score_ptr);
    sprintf(buffer, "Letter score: %d", score_ptr->total_score);
    gtk_label_set_text(GTK_LABEL(ptr), buffer);

    update_check_label(score_ptr->checkA, check_a_label);
    update_check_label(score_ptr->checkB, check_b_label);
    update_check_label(score_ptr->checkC, check_c_label);
    update_check_label(score_ptr->checkD, check_d_label);
    update_check_label(score_ptr->checkE, check_e_label);
    update_check_label(score_ptr->checkF, check_f_label);
    update_check_label(score_ptr->checkG, check_g_label);
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

    GtkWidget *frame_a = gtk_frame_new("Check A");
    check_a_label = gtk_label_new("0 points");
    gtk_container_add(GTK_CONTAINER(frame_a), check_a_label);

    GtkWidget *frame_b = gtk_frame_new("Check B");
    check_b_label = gtk_label_new("0 points");
    gtk_container_add(GTK_CONTAINER(frame_b), check_b_label);

    GtkWidget *frame_c = gtk_frame_new("Check C");
    check_c_label = gtk_label_new("0 points");
    gtk_container_add(GTK_CONTAINER(frame_c), check_c_label);

    GtkWidget *frame_d = gtk_frame_new("Check D");
    check_d_label = gtk_label_new("0 points");
    gtk_container_add(GTK_CONTAINER(frame_d), check_d_label);

    GtkWidget *frame_e = gtk_frame_new("Check E");
    check_e_label = gtk_label_new("0 points");
    gtk_container_add(GTK_CONTAINER(frame_e), check_e_label);

    GtkWidget *frame_f = gtk_frame_new("Check F");
    check_f_label = gtk_label_new("0 points");
    gtk_container_add(GTK_CONTAINER(frame_f), check_f_label);

    GtkWidget *frame_g = gtk_frame_new("Check G");
    check_g_label = gtk_label_new("0 points");
    gtk_container_add(GTK_CONTAINER(frame_g), check_g_label);

    GtkWidget *checkbox = gtk_hbox_new(FALSE, 20);

    gtk_box_pack_start(GTK_BOX(checkbox), frame_a, TRUE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(checkbox), frame_b, TRUE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(checkbox), frame_c, TRUE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(checkbox), frame_d, TRUE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(checkbox), frame_e, TRUE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(checkbox), frame_f, TRUE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(checkbox), frame_g, TRUE, FALSE, 5);

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
