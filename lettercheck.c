#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "trigram_table.h"
#include "ascii.h"

int is_punc(char c) {
    if (c == '.' || c == '?' || c == '!') return 1;
    return 0;
}

int adjacent_check(char* letter, int letter_length, int idx) {
    // check the next three characters after a given idx for a capital letter
    for (int offset = 1; offset < 4; offset++) {
        // avoiding overflow with edge cases when length is (almost) maximum
        if (idx + offset > 192) return -10;
        if (idx + offset >= letter_length && letter_length > 190) return -10;
        if (idx + offset >= letter_length) return 0; // avoids overflow
        int ascii = (int)letter[idx + offset];
        if (ascii >= 65 && ascii <= 90) {
            return 10;
        }
    }

    // no capital found
    return -10;
}

int punc_and_cap(char* letter, int letter_length) {
    int score = 0;

    for (int i = 0; i < letter_length; i++) {
        if (is_punc(letter[i])) {
            score += adjacent_check(letter, letter_length, i);
        }
    }

    // check last char for punctuation
    if (letter_length < 193) {
        if (is_punc(letter[letter_length - 2])) score += 20;
    }

    printf("Score from Check A: %d\n", score);
    return score;
}

extern struct string_table trigram_table[];

int is_trigram(char* str) {
    int char0 = (int)str[0];
    int char1 = (int)str[1];
    int char2 = (int)str[2];

    for (int i = 0; i < 26; i++) {
        int* str_table = trigram_table[i].double_letter_table;

        if (char0 == trigram_table[i].lowercase_char || char0 == trigram_table[i].uppercase_char) {
            // search through a letter's str table to find valid trigram endings
            /* NOTE: string tables in-game are missing control codes to halt iteration. To
            approximate this behavior, we only inlude a stop character at the bottom of the
            z_table.*/
            for (str_table; str_table[0] != CHAR_CONTROL_CODE; str_table += 2) {
                if (char1 == str_table[0] && char2 == str_table[1]) return 1;
            }
        }
    }

    return 0;
}

int get_next_word_idx(char* letter, int letter_length, int current_idx) {
    int idx = current_idx + 1;
    
    while (idx < letter_length - 1) {
        if (letter[idx] == ' ') {
            idx++;
            break;
        } else idx++;
    }
    return idx;
}

int trigram_check(char* letter, int letter_length) {
    int count = 0;

    int i = 0;
    while (i < letter_length - 3) {
        char triplet[3];
        triplet[0] = letter[i];
        triplet[1] = letter[i + 1];
        triplet[2] = letter[i + 2];
        if (is_trigram(triplet) > 0) {
            count++;
        }
        i = get_next_word_idx(letter, letter_length, i);
    }

    printf("Score from Check B: %d\n", count * 3);
    return count * 3;
}

int start_capital_check(char* letter, int letter_length) {
    int score = -10;

    for (int i = 0; i < letter_length; i++) {
        int ascii = (int)letter[i];
        if (ascii >= 65 && ascii <= 90) {
            score = 20;
            break;
        }
    }

    printf("Score from Check C: %d\n", score);
    return score;
}

int repeating_char_check(char* letter, int letter_length) {
    int idx = 0;

    while (idx < letter_length - 3) {
        if (letter[idx] == letter[idx + 1]) {
            if (letter[idx] == letter[idx + 2]) {
                printf("Score from Check D: -50\n");
                return -50;
            } else idx += 2;
        } else idx += 1;
    }
 
    printf("Score from Check D: 0\n");
    return 0;
}

int space_ratio_check(char* letter, int letter_length) {
    int score = -20;
    int spaces = 0;
    int nonspaces = 0;

    for (int i = 0; i < letter_length; i++) {
        if ((int)letter[i] == CHAR_SPACE) spaces++;
        else nonspaces++;
    }

    nonspaces--; // do not count terminator as a nonspace
    if (nonspaces == 0) {
        return score;
    }

    int space_ratio = (spaces * 100) / nonspaces;
    if (space_ratio >= 20) score = 20;

    printf("Score from Check E: %d\n", score);
    return score;
}

int has_punctuation(char* letter, int letter_length) {
    // check the first (length - 75) characters for {'.', '!', '?'}
    // NOTE: this function is only called when a letter has 75 or more characters!
    int idx = letter_length - 75;
    for (int i = 0; i < idx; i++) {
        if (letter[i] == '.' || letter[i] == '!' || letter[i] == '?') {
            return 1;
        }
    }
    return 0;
}

int runon_check(char* letter, int letter_length) {
    int score = 0;

    if (letter_length < 75) {
        printf("Score from Check F: 0\n");
        return 0;
    }

    // if a letter has at least one punctuation mark before the final 75 characters...
    if (!has_punctuation(letter, letter_length)) {
        printf("Score from Check F: 0\n");
        return 0;
    }

    // ...check after each punctuation mark for 75 characters without punctuation
    int i = 0;
    while (i < letter_length) {
        if (letter[i] == '.' || letter[i] == '!' || letter[i] == '?') {
            // if there aren't 75 characters after punctuation, no deduction
            if (i + 75 > letter_length - 2) {
                printf("Score from Check F: 0\n");
                return 0;
            }
            int j = i + 1;
            for (j; j < i + 76; j++) {
                if (letter[j] == '.' || letter[j] == '!' || letter[j] == '?') {
                    i = j;
                    break;
                }
            }
            if (i != j) {
                printf("Score from Check F: -150\n");
                return -150;
            }
            // return -150;
        } else i++;
    }

    printf("Score from Check F: %d\n", score);
    return score;
}

int chunk_check(char* letter, int letter_length) {
    int score = 0;
    int chunks_with_spaces = 0;
    int chunks_to_score = (letter_length - 1) / 32;

    for (int i = 0; i < chunks_to_score; i++) {
        for (int j = 0; j < 32; j++) {
            int idx = (32 * i) + j;
            if (letter[idx] == ' ') {
                chunks_with_spaces++;
                break;
            }
        }
    }

    score -= 20 * (chunks_to_score - chunks_with_spaces);

    printf("Score from Check G: %d\n", score);
    return score;
}

int score_letter(char* letter, int letter_length) {
    int final_score = 0;
    final_score += punc_and_cap(letter, letter_length);
    final_score += trigram_check(letter, letter_length);
    final_score += start_capital_check(letter, letter_length);
    final_score += repeating_char_check(letter, letter_length);
    final_score += space_ratio_check(letter, letter_length);
    final_score += runon_check(letter, letter_length);
    final_score += chunk_check(letter, letter_length);

    return final_score;
}

