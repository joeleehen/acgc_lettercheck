#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "trigram_table.h"
#include "ascii.h"
#include "lettercheck.h"

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
