/*
1) Punctuation and Capitalization
    - check every {. ! ?}, if a capital letter is in the next three spaces, +10 points
        else, -10 points

2) Trigram Check
    - More on this later
    - Each letter has a big fuckoff table of valid two succeeding characters for
        each letter's trigrams.

3) Starting Capital Check
    - If first nonspace character of the letter is a capital letter, + 20 points
        else, -10 points

4) Repeating Characters Check
    - If any letter (character?) is repeated three or more times in sequence, -50
        points and MOVE ON
    - Once we find three repeats, we do NOT need to check for more

5) Space Ratio Check
    - space ratio = (number of spaces * 100) / number of nonspaces
    - +20 points if space ratio is >= 20, -20 if space ratio < 20
    - If there are no nonspaces, -20 points

6) Run-on Sentence Check
    - If the letter has 75 or more characters and there are no punctuation marks in the first
    75 characters, -50 points

7) 32-character Check
    - Split the letter into 32-character group. For every group that does NOT have a space,
        -20 points
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int is_punc(char c) {
    if (c == '.' || c == '?' || c == '!') return 1;
    return 0;
}

int adjacent_check(char letter[], int letter_length, int idx) {
    // check the next three characters after a given idx for a capital letter
    printf("examining: %c", letter[idx]);
    for (int offset = 1; offset < 4; offset++) {
        // avoiding overflow with edge cases when length is (almost) maximum
        if (idx + offset > 192) return -10;
        if (idx + offset >= letter_length && letter_length > 190) return -10;
        if (idx + offset >= letter_length) return 0; // avoids overflow
        printf("%c", letter[idx + offset]);
        int ascii = (int)letter[idx + offset];
        if (ascii >= 65 && ascii <= 90) {
            printf("    adding puncCap");
            return 10;
        }
    }

    // no capital found
    printf("    subtracting puncCap");
    return -10;
}

int punc_and_cap(char letter[], int letter_length) {
    int score = 0;

    for (int i = 0; i < letter_length; i++) {
        if (is_punc(letter[i])) {
            score += adjacent_check(letter, letter_length, i);
            printf("\n");
        }
    }

    // check last char for punctuation
    if (letter_length < 193) {
        if (is_punc(letter[letter_length - 2])) score += 20;
    } else printf("letter too big, not checking last char\n");

    printf("score from check 1: %d\n", score);
    return score;
}

int start_capital_check(char letter[], int letter_length) {
    int score = -10;

    for (int i = 0; i < letter_length; i++) {
        int ascii = (int)letter[i];
        if (ascii >= 65 && ascii <= 90) {
            score = 20;
            break;
        }
    }

    return score;
}

int repeating_char_check(char letter[], int letter_length) {
    int idx = 0;

    while (idx < letter_length - 3) {
        if (letter[idx] == letter[idx + 1]) {
            if (letter[idx] == letter[idx + 2]) {
                return -50;
            } else idx += 2;
        } else idx += 1;
    }
 
    return 0;
}

int space_ratio_check(char letter[], int letter_length) {
    int score = -20;
    int spaces = 0;
    int nonspaces = 0;

    for (int i = 0; i < letter_length; i++) {
        if ((int)letter[i] == 32) spaces++;
        else nonspaces++;
    }

    if (nonspaces == 0) {
        return score;
    }

    int space_ratio = (spaces * 100) / nonspaces;
    if (space_ratio >= 20) score = 20;

    return score;
}

int has_punctuation(char letter[], int letter_length) {
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

int runon_check(char letter[], int letter_length) {
    int score = 0;

    if (letter_length < 75) return 0;

    // if a letter has at least one punctuation mark before the final 75 characters...
    if (!has_punctuation(letter, letter_length)) return 0;

    // ...check after each punctuation mark for 75 characters without punctuation
    int i = 0;
    printf("checking for run on sentence...\n");
    while (i < letter_length) {
        if (letter[i] == '.' || letter[i] == '!' || letter[i] == '?') {
            printf("found punctuation at index %d\n", i);
            // if there aren't 75 characters after punctuation, no deduction
            if (i + 75 > letter_length - 2) {
                printf("there aren't 75 characters left to examine\n");
                return 0;
            }
            int j = i + 1;
            for (j; j < i + 76; j++) {
                if (letter[j] == '.' || letter[j] == '!' || letter[j] == '?') {
                    printf("found another punctuation mark at index %d", j);
                    i = j;
                    break;
                }
            }
            if (i != j) return -150;
            // return -150;
        } else i++;
    }
    return score;
}

int score_letter(char* letter, int letter_length) {
    int final_score = 0;
    final_score += punc_and_cap(letter, letter_length);
    final_score += start_capital_check(letter, letter_length);
    final_score += repeating_char_check(letter, letter_length);
    final_score += space_ratio_check(letter, letter_length);
    final_score += runon_check(letter, letter_length);

    return final_score;
}

int main(int argc, char *argv[])
{
    char *buffer;
    FILE *letter_file;
    int letter_score = 0;

    if (argc == 1) {
        printf("ERROR: no input file specified.\n");
        return 1;
    }

    char * filename = argv[1];
 
    letter_file = fopen(filename, "rb");
    if (letter_file == NULL) {
        printf("ERROR: file %s not found!", filename);
        return 1;
    }

    fseek(letter_file, 0, SEEK_END);
    long letter_size = ftell(letter_file);
    if (letter_size > 192) letter_size = 192;
    fseek(letter_file, 0, SEEK_SET);
    buffer = (char *)malloc(letter_size);

    if (buffer == NULL) {
        printf("ERROR: couldn't read file %s into memory!\n", filename);
        return 1;
    }

    fread(buffer, 1, letter_size, letter_file);

    if (letter_size == 192) {
        letter_size++;
        buffer = realloc(buffer, letter_size);
        buffer[192] = '\0';
    }

    if (!buffer) {
        printf("ERROR: could not read file into memory buffer!");
        return 1;
    }

    letter_score = score_letter(buffer, letter_size);

    printf("letter score: %d\n", letter_score);


    free(buffer);
    fclose(letter_file);

    return 0;
}
