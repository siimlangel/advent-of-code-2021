#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <math.h>


int cmpfunc(const void *a, const void *b)
{
    return (*(int*) a - *(int *) b);
}

void dump(int *nums, int size)
{
    for(size_t i = 0; i < size; i++) {
        printf("%d, ", nums[i]);
    }
    printf("\n");
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        printf("Usage: ./main [file_name.ext]\n");
        exit(1);
    }

    char *FILENAME = *(argv + 1);
    FILE *file;
    char *line = NULL;
    size_t len = 0;
    file = fopen(FILENAME, "r");
    assert(file != NULL);

    // Count how many nums in file

    getline(&line, &len, file);
    fclose(file);

    // 1 cus 1 less comma than nums
    int nums_size = 1;
    size_t i = 0;
    while(line[i] != '\0') {
        nums_size += line[i] == ',';
        i++;
    }
    
    int *nums = malloc(sizeof(int) * nums_size);
    
    // Read in all nums
    char *rest = NULL;
    char *token = strtok_r(line, ",", &rest);
    i = 0;
    while (token != NULL) {
        nums[i++] = atoi(token);
        token = strtok_r(NULL, ",", &rest);
    }

    // Sort and find median
    qsort(nums, nums_size, sizeof(int), cmpfunc);


    // the median is the point that minimizes the sum of absolute differences
    int median = nums[nums_size / 2];
    
    uint64_t sum = 0;
    // Find sum of absolute differences
    for(i = 0; i < nums_size; i++) {
        sum += abs(nums[i] - median);
    }
    
    float sum2 = 0;
    
    for(i = 0; i < nums_size; i++) {
        sum2 += nums[i];
    }
    // the mean does the same but also takes outliers into account
    int mean = floor(sum2 / nums_size);
    
    uint64_t sum_part2 = 0;
    int diff;
    for(i = 0; i < nums_size; i++) {
        diff = abs(nums[i] - mean);
        int s = (diff * (diff + 1)) / 2;
        sum_part2 += s;
    }

    printf("part1: %llu\n", (unsigned long long) sum);
    printf("part2: %llu\n", (unsigned long long) sum_part2);

    free(nums);
    return 1;
}
