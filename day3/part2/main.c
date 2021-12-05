#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

size_t num_bits_in_line(char *line) 
{
    size_t i = 0;
    while (line[i] != '\n') {
        i++;
    }

    return i;
}

int bin_str_to_int(char *str, size_t len) {
    int res = 0;

    for (size_t i = 0; i < len; i++) {
        res <<= 1;
        if (str[i] == '1') res ^= 1;
    }
    return res;
}

typedef int (*compare_cb)(double proportion_of_ones);

int oxygen_cb(double proportion_of_ones)
{
    if (proportion_of_ones >= 0.5) return 1;
    return 0;
}

int co2_cb(double proportion_of_ones)
{
    if (proportion_of_ones < 0.5) return 1;
    return 0;
}

int main (int argc, char **argv)
{
    if (argc != 3) {
        printf("%d\n", argc);
        printf("Usage: ./main [input_file_name.ext] [CO2 | O2]\n");
        exit(1);
    }

    char *FILENAME = *(argv + 1);
    char *mode = *(argv + 2);

    FILE *file;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    file = fopen(FILENAME, "r");
    assert(file != NULL);
    
    compare_cb cb = co2_cb;

    if (strcmp(mode, "O2") == 0) {
        cb = oxygen_cb;
    }
    
    // Get number of bits in one number of the data
    read = getline(&line, &len, file);
    size_t num_bits = num_bits_in_line(line);
 
    rewind(file); 
    
    int num_lines = 0; 
    
    // Calculate number of lines in file to get array size
    while((read = getline(&line, &len, file)) != -1) {
        num_lines++;
    }
   
    rewind(file); 

    // Allocate memory for the array containing every line
    char **data = malloc(sizeof(char*) * num_lines);

    // read all the lines into an array
    for(size_t i = 0; i < num_lines; i++) {
        read = getline(&line, &len, file);

        data[i] = malloc(sizeof(char) * len);
        strcpy(data[i], line);
    }

    
    
    int num_ones;
    int num_nums_remaining;
    double proportion_of_ones;
    char *final_string = malloc(sizeof(char) * 32);

    for(size_t i = 0; i < num_bits; i++) {
        num_ones = 0;
        num_nums_remaining = 0;

        // Calculate number of ones in ith position and the number of nums remaining
        for(size_t j = 0; j < num_lines; j++) {
            if(strcmp(data[j], "X") == 0) continue;
            
            if (data[j][i] == '1') num_ones++;
            num_nums_remaining++;
        }

        proportion_of_ones = num_ones / (double) num_nums_remaining;

        if (num_nums_remaining == 1) {
            break;
        }
        // Look at every binary string with i being the bit being looked at
        for(size_t j = 0; j < num_lines; j++) {
            if (strcmp(data[j], "X") == 0) {
                continue;
            }
            // More ones or equal amount or less depending on cb
            if (cb(proportion_of_ones)) {
                // If there are more ones but this string has a 0 at i position
                if(data[j][i] != '1') {
                    strcpy(data[j], "X");
                }
            // More zeros
            } else {
                // If there are more zeros but this string has a 1 at i position
                if(data[j][i] != '0') {
                    strcpy(data[j], "X");
                }
            }
        }
    }

    for(size_t e = 0; e < num_lines; e++) {
        if (strcmp(data[e], "X") != 0) {
            strcpy(final_string, data[e]);
            break;
        }
    }

    int res = bin_str_to_int(final_string, num_bits);

    if (strcmp(mode, "CO2") == 0) {
        printf("CO2 scrubber rating: %d\n", res);
    } else {
        printf("Oxygen generator rating: %d\n", res);
    }


    for (size_t i = 0; i < num_lines; i++) {
        free(data[i]);
    }
    free(final_string);
    free(data);
    free(file);
    return 0;
}
