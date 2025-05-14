#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include "trigram_table.h"
#include "ascii.h"
#include "lettercheck.h"

GtkWidget *txt;
GtkWidget *length_label;
GtkWidget *check_a_label;
GtkWidget *check_b_label;
GtkWidget *check_c_label;
GtkWidget *check_d_label;
GtkWidget *check_e_label;
GtkWidget *check_f_label;
GtkWidget *check_g_label;
GtkTextIter start, end;

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

char * get_letter_text(GtkWidget *wid, int key_pressed, gchar* c) {
    GtkTextBuffer *letter_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(wid));
    int letter_length = gtk_text_buffer_get_char_count(GTK_TEXT_BUFFER(letter_buffer));
    gtk_text_buffer_get_iter_at_offset(letter_buffer, &start, 0);

    /* NOTE: this assumes all characters are a single byte long. this glosses over some 
    multibyte UTF-8 skullduggery but is inaccurate to the original game.*/

    if (key_pressed == CHAR_BACKSPACE) {
        printf("received backspace\n");
        gtk_text_buffer_get_iter_at_offset(letter_buffer, &end, letter_length - 1);
        gtk_text_buffer_get_iter_at_offset(letter_buffer, &start, 0);
        char* letter = gtk_text_buffer_get_text(GTK_TEXT_BUFFER(letter_buffer), &start, &end, FALSE);
        return letter;
    }

    gunichar char_pressed = gdk_keyval_to_unicode(key_pressed);

    printf("char pressed: %c (key pressed: %d)\n", char_pressed, key_pressed);
    if (!g_unichar_isprint(char_pressed) && key_pressed != CHAR_ENTER || letter_length > 193) {
        gtk_text_buffer_get_iter_at_offset(letter_buffer, &end, -1);
        char *letter = gtk_text_buffer_get_text(GTK_TEXT_BUFFER(letter_buffer), &start, &end, FALSE);
        return letter;
    }

    gtk_text_buffer_get_iter_at_offset(letter_buffer, &end, 192); // will not overflow if buffer < 192 chars

    // TextIters must be set every time the buffer is changed!
    gtk_text_buffer_insert(letter_buffer, &end, c, 1);
    gtk_text_buffer_get_iter_at_offset(letter_buffer, &start, 0);
    gtk_text_buffer_get_iter_at_offset(letter_buffer, &end, 192);
    char* letter = gtk_text_buffer_get_text(GTK_TEXT_BUFFER(letter_buffer), &start, &end, FALSE);

    // g_free(letter_buffer);

    return letter;
}

gboolean get_letter_score(GtkWidget *wid, GdkEventKey *event, gpointer ptr) {
    char buffer[20];
    char length_buffer[20];
    GtkTextBuffer *letter_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(wid));
    int letter_length = gtk_text_buffer_get_char_count(GTK_TEXT_BUFFER(letter_buffer));

    // enforce 192-character limit (except for backspaces)
    if (letter_length == 192) {
        if (event->keyval != CHAR_BACKSPACE) return TRUE;
    }

    // get buffer and score
    char* letter = get_letter_text(txt, event->keyval, event->string);
    int letter_score = score_letter(letter, strlen(letter), score_ptr);
    sprintf(buffer, "Letter score: %d", score_ptr->total_score);
    gtk_label_set_text(GTK_LABEL(ptr), buffer);

    // update composite score labels
    update_check_label(score_ptr->checkA, check_a_label);
    update_check_label(score_ptr->checkB, check_b_label);
    update_check_label(score_ptr->checkC, check_c_label);
    update_check_label(score_ptr->checkD, check_d_label);
    update_check_label(score_ptr->checkE, check_e_label);
    update_check_label(score_ptr->checkF, check_f_label);
    update_check_label(score_ptr->checkG, check_g_label);

    // update length label
    if (event->keyval == CHAR_BACKSPACE && letter_length > 0) {
        letter_length--;
    } else if (letter_length != 0) {
        letter_length++;
    }
    sprintf(length_buffer, "Characters: %d/192", letter_length);
    gtk_label_set_text(GTK_LABEL(length_label), length_buffer);

    if (event->keyval == CHAR_BACKSPACE) return FALSE;
    return TRUE;
}

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);
    GtkWidget *win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(win), 600, 700);
    g_signal_connect(win, "delete_event", G_CALLBACK(end_program), NULL);

    GtkWidget *box = gtk_vbox_new(FALSE, 10);
    GtkWidget *title_label = gtk_label_new("ACGC Letter Grader");

    length_label = gtk_label_new("Characters: 0/192");
    gtk_misc_set_alignment(GTK_MISC(length_label), 0.95, 0.0);

    // txt = gtk_entry_new();
    txt = gtk_text_view_new();
    gtk_widget_set_size_request(txt, 300, 400);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(txt), GTK_WRAP_WORD);

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

    g_signal_connect(txt, "key-press-event", G_CALLBACK(get_letter_score), score_label);

    gtk_box_pack_start(GTK_BOX(box), title_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), length_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), txt, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), score_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), checkbox, FALSE, FALSE, 0);
    gtk_container_add(GTK_CONTAINER(win), box);

    gtk_widget_show_all(win);
    gtk_main();
    return 0;
}
