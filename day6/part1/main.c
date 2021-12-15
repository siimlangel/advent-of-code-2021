#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>


#define TABLE_SIZE 9

void spawn(uint64_t *fishes)
{
    uint64_t next_fishes[TABLE_SIZE];
    memset(next_fishes, 0, sizeof(uint64_t) * TABLE_SIZE);

    next_fishes[8] += fishes[0];
    next_fishes[6] += fishes[0];
    size_t i;
    for(i = 1; i < TABLE_SIZE; i++) {
        next_fishes[i - 1] += fishes[i];
    }
    memcpy(fishes, next_fishes, sizeof(uint64_t) * TABLE_SIZE);
}

void dump_fishes(uint64_t *fishes)
{
    size_t i;
    for(i = 0; i < TABLE_SIZE; i++) {
        printf("%llu, ", (unsigned long long)fishes[i]);
    }
    printf("\n");
    
}

uint64_t sum_fishes(uint64_t *fishes)
{
    uint64_t sum = 0;
    for(size_t i = 0; i < TABLE_SIZE; i++) {
        sum += fishes[i];
    }
    return sum;
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        printf("Usage: ./main [file_name.ext]\n");
        exit(1);
    }
    char *FILENAME = *(argv + 1);
    FILE *file = fopen(FILENAME, "r");
    assert(file != NULL);

    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    read = getline(&line, &len, file);
    assert(read != -1);
    fclose(file);


    uint64_t fishes[TABLE_SIZE];
    memset(fishes, 0, sizeof(uint64_t) * TABLE_SIZE);

    
    char *token = strtok(line, ",");
    uint64_t fish;
    while (token != NULL) {
        fish = (uint64_t)atoi(token);
        fishes[fish]++;
        token = strtok(NULL, ",");
    }

    for(size_t i = 0; i < 256; i++) {
        spawn(fishes);
    }
    uint64_t total = sum_fishes(fishes);
    printf("Total: %llu\n", (unsigned long long)total);

    if(line) free(line);
    return 0;
}
