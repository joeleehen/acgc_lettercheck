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
    75 characters, -150 points

7) 32-character Check
    - Split the letter into 32-character group. For every group that does NOT have a space,
        -20 points
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
                return -150;
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

int score_letter(char* letter, int letter_length) {
    int final_score = 0;
    final_score += start_capital_check(letter, letter_length);
    final_score += repeating_char_check(letter, letter_length);
    final_score += space_ratio_check(letter, letter_length);

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
    fseek(letter_file, 0, SEEK_SET);
    // printf("the letter is of size %d\n", letter_size);
    buffer = (char *)malloc(letter_size);

    if (buffer == NULL) {
        printf("ERROR: couldn't read file %s into memory!\n", filename);
        return 1;
    }

    fread(buffer, 1, letter_size, letter_file);

    if (!buffer) {
        printf("ERROR: could not read file into memory buffer!");
        return 1;
    }

    letter_score = score_letter(buffer, letter_size);

    printf("letter score: %d", letter_score);


    fclose(letter_file);

    return 0;
}
