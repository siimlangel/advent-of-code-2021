#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#define LEN1 2
#define LEN4 4
#define LEN7 3
#define LEN8 7
#define COLS 256
#define ROWS 9


void reset_table(int table[][COLS])
{
    size_t i;
    for(i = 0; i < ROWS; i++) {
        memset(table[i], 0, sizeof(int) * COLS);
    }
}

void assign_table(int table[][COLS], int digit, char *signal, size_t signal_len)
{
    size_t i;
    int ascii;
    for (i = 0; i < signal_len; i++) {
        ascii = (int) signal[i];
        table[digit][ascii] = 1;
    }
}

void get_signal_digits(char* signal, int table[][COLS])
{
    if (signal == NULL) return;
    size_t signal_len = strlen(signal);
    switch(signal_len) {
        case LEN1:
            assign_table(table, 1, signal, signal_len);
            break;
        case LEN4:
            assign_table(table, 4, signal, signal_len);
            break;
        case LEN7:
            assign_table(table, 7, signal, signal_len);
            break;
        case LEN8:
            assign_table(table, 8, signal, signal_len);
            break;
        default:
            break;
    }
}

void dump_table(int table[][COLS])
{
    size_t i;
    size_t j;
    for(i = 0; i < ROWS; i++) {
        printf("%zu: ", i);
        for(j = 0; j < COLS; j++) {
            if(table[i][j]) printf("%c", (char)j);
        }
        printf("\n");
    }
}

bool is_output_in_table(char *output, int table[][COLS])
{
    if (output == NULL) return false;
    
    size_t output_len = strlen(output);
    size_t i;
    int row;

    
    switch(output_len) {
        case LEN1:
            row = 1;
            break;
        case LEN4:
            row = 4;
            break;
        case LEN7:
            row = 7;
            break;
        case LEN8:
            row = 8;
            break;
        default:
            return false;
    }
    int ascii;
    for(i = 0; i < output_len; i++) {
        ascii = (int) output[i];
        if (!table[row][ascii]) return false;
    }
    
    return true;
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        printf("Usage: ./main [file_name.txt]\n");
        exit(1);
    }
    
    int digit_table[ROWS][COLS];
    size_t i;

    char *FILENAME = *(argv + 1);
    FILE *file;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    file = fopen(FILENAME, "r");
    
    char *signals;
    char *outputs;
    char *rest;
    i = 0;
    int total = 0;
    while((read = getline(&line, &len, file)) != -1) {
        reset_table(digit_table);
        signals = strtok_r(line, "|", &rest);
        outputs = strtok_r(NULL, "|", &rest);
        rest = NULL;
        // Find which letters are on for each digit
        // and mark that digit in the table
        char *signal;
        signal = strtok_r(signals, " ", &rest);
        while(signal != NULL) {
            get_signal_digits(signal, digit_table);
            signal = strtok_r(NULL, " ", &rest);
        }
        
        // Check if an output is present in the signals
 
        rest = NULL;
        char *output;
        output = strtok_r(outputs, " \n", &rest);
        while(output != NULL) {
            
            if(is_output_in_table(output, digit_table)) {
                total++;
            }

            output = strtok_r(NULL, " \n", &rest);
        }

        
        
    }
    printf("Answer: %d\n", total);

    if(line) free(line);
    fclose(file);
    return 1;
}
