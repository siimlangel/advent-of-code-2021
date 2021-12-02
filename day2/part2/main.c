#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>


void get_movement(char **direction, int *amount, char *line)
{
    char line_copy[32];
    strcpy(line_copy, line);

    char *new_dir = strtok(line_copy, " ");
    char *new_amount = strtok(NULL, " ");

    *amount = atoi(new_amount);
    *direction = new_dir;
}


int main(int argc, char **argv)
{
    if(argc != 2) { 
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
    
    int horizontal_pos = 0;
    int depth = 0;
    int aim = 0;
    char *direction = "forward";
    int amount = 0;


    while((read = getline(&line, &len, file)) != -1) {  
        get_movement(&direction, &amount, line);


        if (strcmp(direction, "forward") == 0) {
            horizontal_pos += amount;
            depth += aim * amount;
        } else if (strcmp(direction, "down") == 0) {
            aim += amount;
        } else if (strcmp(direction, "up") == 0) {
            aim -= amount;
        } else {
            printf("direction: %s not implemented\n", direction);
            exit(1);
        }
    }

    printf("Final positions: \n");
    printf("    horizontal: %d\n", horizontal_pos);
    printf("    depth:      %d\n", depth);
    printf("    product:    %d\n", depth * horizontal_pos);

    return 0;
}
