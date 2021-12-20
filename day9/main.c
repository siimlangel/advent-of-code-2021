#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#define BASIN_CAP 1000


void read_input(char *FILENAME, int ***arr, int *row_ptr, int *col_ptr)
{
    FILE *file = fopen(FILENAME, "r");
    assert(file != NULL);

    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int rows = 1;
    int cols = 0;
    read = getline(&line, &len, file);

    size_t i = 0;
    while(line[i++] != '\n') {
        cols++;
    }
    

    while((read = getline(&line, &len, file)) != -1) {
        rows++;    
    }
    *arr = malloc(sizeof(int *) * rows);

    for(i = 0; i < rows; i++) {
        (*arr)[i] = malloc(sizeof(int) * cols);
    }

    rewind(file);
    i = 0;
    size_t j;
    while((read = getline(&line, &len, file)) != -1) {
        j = 0;
        while(j < cols) {
            (*arr)[i][j] = line[j] - '0';
            j++;
        }
        i++;
    }

    *row_ptr = rows;
    *col_ptr = cols;
    fclose(file);
    if (line) free(line);
}

void dump(int **arr, int rows, int cols)
{
    for(size_t i = 0; i < rows; i++) {
        for(size_t j = 0; j < cols; j++) {
            printf("%d", arr[i][j]);
        }
        printf("\n");
    }
}

int add_to_sum(int a, int b)
{
    return a + b + 1;
}

void part1(int **arr, int rows, int cols)
{
    int sum = 0;
    // Get all corners
    // Top left
    if (arr[0][0] < arr[0][1] && arr[0][0] < arr[1][0]) {
        sum = add_to_sum(sum, arr[0][0]);
    }
    // Top right
    if (arr[0][(cols - 1)] < arr[0][(cols - 2)] && arr[0][(cols - 1)] < arr[1][(cols - 1)]) {
        sum = add_to_sum(sum, arr[0][(cols - 1)]);
    }
    // Bot left
    if (arr[(rows - 1)][0] < arr[(rows - 2)][0] && arr[(rows - 1)][0] < arr[(rows - 1)][1]) {
        sum = add_to_sum(sum, arr[(rows - 1)][0]);
    }
    // Bot right
    if (arr[(rows - 1)][(cols - 1)] < arr[(rows - 1)][(cols - 2)] && arr[(rows - 2)][(cols - 1)]) {
        sum = add_to_sum(sum, arr[(rows - 1)][(cols - 1)]);
    }

    // Get top and bottom rows without corners

    size_t i;
    size_t j;
    int elem;
    for(i = 1; i < (cols - 1); i++) {
        elem = arr[0][i];
        // compare left bot and right
        if (elem < arr[0][i - 1] && elem < arr[1][i] && elem < arr[0][i + 1]) {
            sum = add_to_sum(sum, elem);
        }
    }

    for(i = 1; i < (cols - 1); i++) {
        elem = arr[(rows) - 1][i];
        // Compare left top and right
        if (elem < arr[(rows) - 1][i - 1] && elem < arr[(rows) - 2][i] && elem < arr[(rows - 1)][i + 1]) {
            sum = add_to_sum(sum, elem);
        }
    }

    // Get left and right columns without corners

    for(j = 1; j < (rows - 1); j++) {
        elem = arr[j][0];
        // Compare top right and bot
        if (elem < arr[j - 1][0] && elem < arr[j][1] && elem < arr[j + 1][0]) {
            sum = add_to_sum(sum, elem);
        }
    }

    for(j = 1; j < (rows - 1); j++) {
        elem = arr[j][(cols - 1)];
        // Compare top left and bot
        if (elem < arr[j - 1][(cols - 1)] && elem < arr[j][(cols - 2)] && elem < arr[j + 1][(cols - 1)]) {
            sum = add_to_sum(sum, elem);
        }
    }

    int top, right, bot, left;
    // Get everything except border rows and cols
    for(i = 1; i < (rows - 1); i++) {
        for(j = 1; j < (cols - 1); j++) {
            elem = arr[i][j];
            top = arr[i - 1][j];
            right = arr[i][j + 1];
            bot = arr[i + 1][j];
            left = arr[i][j - 1];
            // Check top right bot left
            if (elem < top && elem < right && elem < bot && elem < left) {
                sum = add_to_sum(sum, elem);
            }
        }
    }
    printf("Part 1: %d\n", sum);
}

void dfs(int **arr, int row, int col, int rows, int cols, int **visited, int *basin_size)
{
    if (visited[row][col] == 1) return;

    (*basin_size)++;
    visited[row][col] = 1;
    
    int dirs[][4] = {
        {-1, 0}, // top
        {0, 1}, // right
        {1, 0}, // bot
        {0, -1}  // left
    };
    
    int n_row, n_col;
    for(size_t i = 0; i < 4; i++) {
        n_row = row + dirs[i][0];
        n_col = col + dirs[i][1];
        if (n_row >= 0 && n_row < rows && n_col >= 0 && n_col < cols) {
            if(arr[n_row][n_col] >= arr[row][col] && arr[n_row][n_col] < 9) {
                dfs(arr, n_row, n_col, rows, cols, visited, basin_size);
            }
        } 
    }

}

void dump_visited(int **visited, int rows, int cols)
{
    for(size_t i = 0; i < rows; i++) {
        for(size_t j = 0; j < cols; j++) {
            if (visited[i][j]) printf("*");
            else printf(".");
        }
        printf("\n");
    }
}

int cb(const void *a, const void *b) {
    return  (*(int*) a - *(int*)b);
}

void part2(int **arr, int rows, int cols)
{
    int dirs[][4] = {
        {-1, 0}, // top
        {0, 1}, // right
        {1, 0}, // bot
        {0, -1}  // left
    };

    int **visited = malloc(sizeof(int*) * rows);
    for(size_t i = 0; i < rows; i++) {
        visited[i] = malloc(sizeof(int) * cols);
        memset(visited[i], 0, sizeof(int) * cols);
    }

    int row, col;
    int n_row, n_col;
    bool is_min;
    int basin_size = 0;
    
    int basins[BASIN_CAP] = {0};
    int basin_count = 0;

    for(row = 0; row < rows; row++) {
        for (col = 0; col < cols; col++) {
            is_min = true;
            for(size_t i = 0; i < 4; i++) {
                n_row = row + dirs[i][0];
                n_col = col + dirs[i][1];
                // Check out of bounds
                if (n_row >= 0 && n_row < rows && n_col >= 0 && n_col < cols) {
                    if (arr[row][col] >= arr[n_row][n_col]) {
                        is_min = false;
                        break;
                    } 
                }
            }

            if (is_min) {
                basin_size = 0;
                dfs(arr, row, col, rows, cols, visited, &basin_size);
                basins[basin_count++] = basin_size;
            }
        }
    }
    qsort(basins, BASIN_CAP, sizeof(int), cb);
    unsigned long long ans = 1;
    for(size_t i = BASIN_CAP - 1; i >= BASIN_CAP - 3; --i) {
        ans *= basins[i];
    }
    printf("Part2: %llu\n", ans);
}


int main(int argc, char **argv)
{
    if (argc != 2) {
        printf("Usage: ./main [file_name.txt]\n");
        exit(1);
    }
    
    char *FILENAME = *(argv + 1);
    int **arr;
    int rows, cols;

    read_input(FILENAME, &arr, &rows, &cols);
    
    part1(arr, rows, cols);
    part2(arr, rows, cols);


    for(size_t i = 0; i < rows; i++) {
        free(arr[i]);
    }
    free(arr);
    return 1;
}
