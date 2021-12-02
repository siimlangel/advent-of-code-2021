#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>



int main(int argc, char **argv)
{

    if (argc != 2) {
        printf("Usage: ./main [input_file_name.ext]\n");
        exit(1);
    }

    char *FILENAME = *(argv + 1);


    FILE *file;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    file = fopen(FILENAME, "r");    
    assert(file != NULL);
   
    // To see if there is an increase just compare element to previous element
    // Expecting to only have positive integers 
    int prev = -1;
    int count_increasing = 0;
    while ((read = getline(&line, &len, file)) != -1) {
        int measure = atoi(line);
        if (prev != -1) {
            count_increasing += (measure > prev);
        }
        prev = measure;
    }
    
    printf("%d\n", count_increasing);

    fclose(file);
    if (line) free(line);

    return 0;
}
