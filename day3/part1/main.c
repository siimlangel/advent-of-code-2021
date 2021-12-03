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

int main (int argc, char **argv)
{
    if (argc != 2) {
        printf("Usage: ./main [input_file_name.ext]");
        exit(1);
    }

    char *FILENAME = *(argv + 1);

    FILE *file;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    file = fopen(FILENAME, "r");
    assert(file != NULL);

    
    read = getline(&line, &len, file);
    printf("num bits in each line: %zu\n", num_bits_in_line(line));
    size_t num_bits = num_bits_in_line(line);

    int *freq = malloc(sizeof(int) * num_bits);

    for (size_t i = 0; i < num_bits; i++) {
        freq[i] = 0;
    }
    
    rewind(file); 
    
    int num_lines = 0; 

    while((read = getline(&line, &len, file)) != -1) {
        size_t i = 0;
        while (line[i] != '\n') {
            if (line[i] == '1') {
                freq[i] += 1;
            }
            i++;
        }
        num_lines++;
    }
    
    char *gamma_rate = malloc(sizeof(char) * num_bits);
    char *epsilon_rate = malloc(sizeof(char) * num_bits);

    for (int i = num_bits - 1; i >= 0; i--) {
        double frequency = freq[i] / (double) num_lines;
        if (frequency < 0.5) {
            gamma_rate[i] = '0';
            epsilon_rate[i] = '1';
        } else {
            gamma_rate[i] = '1';
            epsilon_rate[i] = '0';
        }
    }

    int gamma_int = bin_str_to_int(gamma_rate, num_bits);
    int epsilon_int = bin_str_to_int(epsilon_rate, num_bits);
    

    printf("Gamma: %s = %d\n", gamma_rate, gamma_int);
    printf("Epsilon: %s = %d\n", epsilon_rate, epsilon_int);
    
    printf("Answer: %d\n", gamma_int * epsilon_int);
    free(gamma_rate);
    free(epsilon_rate);
    free(file);
    free(freq);
    return 0;
}
