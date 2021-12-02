#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


int main(int argc, char **argv)
{
    if (argc != 2) {
        printf("Usage ./main [input_file_name.ext]\n");
        exit(1);
    }

    char *FILENAME = *(argv + 1);


    FILE *file;
    char *line;
    size_t len = 0;
    ssize_t read;
    char c;
    size_t line_count = 0;
    int found = 0;

    file = fopen(FILENAME, "r");
    assert(file != NULL);
   
    // Count lines to malloc array 
    while((c = getc(file)) != EOF) {
        if (c == '\n') line_count++;
    }
    rewind(file);

    int *nums = malloc(sizeof(int) * line_count);

    // Read numbers into nums
    size_t i = 0;
    while ((read = getline(&line, &len, file)) != -1) {
        nums[i] = atoi(line);
        i++;
    }
    
    // For a sliding window of 3 the middle 2 elements always overlap
    // So we can just check the first element of the first window
    // and the last element of the second window
    for (size_t i = 0; i < line_count - 3; i++) {
        found += (nums[i + 3] > nums[i]);
    }

    printf("Found %d increasing\n", found);

    free(nums);
    fclose(file);
    if (line) free(line);
    return 0;
}
