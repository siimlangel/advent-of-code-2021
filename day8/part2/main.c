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
#define ROWS 10


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
// Check if first has all chars that second has
bool has_all_chars(char *first, char *second, int len_first, int len_second)
{
    int count = 0;
    size_t i;
    int second_table[COLS];



    // Gonna remember this line for the rest of my life
    // took me an hour to debug why this wasn't working :)
    memset(second_table, 0, sizeof(int) * COLS);




    for (i = 0; i < len_second; i++) {
        second_table[(int) second[i]] = 1;
    }
    for(i = 0; i < len_first; i++) {
        if (second_table[(int) first[i]] != 1) return false;
    }

    return true;

}

void fill_table(char* signals, int table[][COLS])
{
    char *signal_arr[10];
    char *one, *four, *seven, *eight;
    char *rest;
    char *signal = strtok_r(signals, " ", &rest);
    size_t i = 0;
    size_t signal_len;
    while (signal != NULL) {
        signal_arr[i] = signal;
        signal_len = strlen(signal);
        if (signal_len == LEN1) {
            one = signal;
            assign_table(table, 1, one, LEN1);
        } else if (signal_len == LEN7) {
            seven = signal;
            assign_table(table, 7, seven, LEN7);
        } else if(signal_len == LEN4) {
            four = signal;
            assign_table(table, 4, four, LEN4);
        } else if(signal_len == LEN8) {
            eight = signal;
            assign_table(table, 8, eight, LEN8);
        }
        signal = strtok_r(NULL, " ", &rest);
        i++;
    }
    char three[6] = "";
    // 2, 3, 5 all have 5 chars
    // if one of them has all the chars that 1 has its definitely a 3
    for(i = 0; i < 10; i++) {
        signal_len = strlen(signal_arr[i]);
        if (signal_len == 5) {
            if (has_all_chars(one, signal_arr[i], LEN1, 5)) {
                assign_table(table, 3, signal_arr[i], 5);
                strncpy(three, signal_arr[i], 5);
                three[5] = '\0';
                break;
            }
        }
    }
    
    // 0, 6, 9 all have 6 digits
    // if one of them has all the chars that 4 has its definitely a 9
    char nine[7] = "";
    for(i = 0; i < 10; i++) {
        signal_len = strlen(signal_arr[i]);
        if (signal_len == 6) {
            if (has_all_chars(four, signal_arr[i], LEN4, 6)) {
                assign_table(table, 9, signal_arr[i], 6);
                strncpy(nine, signal_arr[i], 6);
                nine[6] = '\0';
                break;
            }
        }
    }

    // Finding 0 and 6
    // If its a nine continue
    // now it is 0 if it has all the chars1 has otherwise it is a 6
    char six[7] = "";
    for(i = 0; i < 10; i++) {
        signal_len = strlen(signal_arr[i]);
        if (strcmp(nine, signal_arr[i]) == 0) continue;
        if (signal_len == 6) {
            if (has_all_chars(one, signal_arr[i], LEN1, 6)) {
                assign_table(table, 0, signal_arr[i], 6);
            } else {
                assign_table(table, 6, signal_arr[i], 6);
                strncpy(six, signal_arr[i], sizeof(six));
                six[6] = '\0';
            }
      
         }
    }
    // Finally 2 an 5
    // If its 3 continue
    // Else if it six has all of the digits then its a 5 else 2
    for(i = 0; i < 10; i++) {
        signal_len = strlen(signal_arr[i]);
        if (strcmp(three, signal_arr[i]) == 0) continue;
        if (signal_len == 5) {
            if (has_all_chars(signal_arr[i], six, 5, 6)) {
                assign_table(table, 5, signal_arr[i], 5);
            } else {
                assign_table(table, 2, signal_arr[i], 5);
            }
      
         }
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

bool found_in_table(int digit, char *output, int table[][COLS])
{
    if(output == NULL) return false;
    int lens[] = {6, 2,5, 5, 4, 5, 6, 3, 7, 6};

    if(lens[digit] != strlen(output)) return false;

    size_t output_len = strlen(output);
    for(size_t i = 0; i < output_len; i++) {
        int ascii = (int) output[i];
        if (table[digit][ascii] != 1) return false;
    }
    return true;
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        printf("Usage: ./main [file_name.txt]\n");
        exit(1);
    }
    
    // 2D array where the row represents the digit
    // columns have a one at [ascii code of signal] if those characters have to be present
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
    char *line_cpy;
    while((read = getline(&line, &len, file)) != -1) {
        // Create copy of line
        reset_table(digit_table);
            
        // Find which combo of chars corresponds to 2,3,5 and 0, 6, 9
        signals = strtok_r(line, "|", &rest);
        outputs = strtok_r(NULL, "|", &rest);
        fill_table(signals, digit_table);


        

        // Handle signals
 
        rest = NULL;
        char *output;
        output = strtok_r(outputs, " \n", &rest);
        int num = 0;
        int x = 1000;
        while(output != NULL) {
            for(size_t j = 0; j < 10; j++) {
                if (found_in_table(j, output, digit_table)) {
                    num += x * j;
                    break;
                }
            }

            x = x / 10;
            output = strtok_r(NULL, " \n", &rest);
        }

        total += num;
        
    }
    printf("Answer: %d\n", total);
    if(line) free(line);
    fclose(file);
    return 1;
}
