#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define BOARD_SIZE 25
#define ROW_SIZE 5
#define ROW 0
#define COL 1


// Count the number of pulls neede for a row or col to win
int count_nums_for_bingo(int *arr, int* bingo_nums_cache, int len_bingo_nums_cache)
{
    int max = -1;
    for(int i = 0; i < ROW_SIZE; i++) {
        // If number isnt in bingo nums
        if (arr[i] < 0
            || arr[i] >= len_bingo_nums_cache
            || bingo_nums_cache[arr[i]] == -1) {
            return -1;
        } else {
            if (bingo_nums_cache[arr[i]] > max){
                max = bingo_nums_cache[arr[i]];
            }        
        }
    }
    return max;
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        printf("Usage ./main [input_file_name.ext]\n");
        exit(1);
    }
    
    char *FILENAME = *(argv + 1);
    FILE *file;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    file = fopen(FILENAME, "r");
    assert(file != NULL);

    // Read in the line with bingo numbers
    read = getline(&line, &len, file);
    
    // Find how many numbers are on the line
    size_t i = 0;
    size_t comma_count = 0;
    while (line[i] != '\n' && line[i] != '\0') {
        if (line[i] == ',') {
            comma_count++;
        }
        i++;
    }

    // There are comma_count + 1 numbers
    size_t len_bingo_nums = comma_count + 1;
    int *bingo_numbers = malloc(sizeof(int) * len_bingo_nums);

    // Split string on ',' and read all the numbers into the array
    
    char *curr = strtok(line, ",");
    i = 0;
    while (curr != NULL) {
        bingo_numbers[i++] = atoi(curr);
        curr = strtok(NULL, ",");
    }

    // Find max bingo number
    int max = -1;
    for(i = 0; i < len_bingo_nums; i++) {
        if (bingo_numbers[i] > max) {
            max = bingo_numbers[i];
        }
    }
    int len_cached_bingo_nums = max + 1;
    int *cached_bingo_nums = calloc(len_cached_bingo_nums, sizeof(int));
    // Add order that bingo num appeared in
    for (i = 0; i < len_bingo_nums; i++) {
        cached_bingo_nums[i] = -1;
    }

    for(i = 0; i < len_bingo_nums; i++) {
        cached_bingo_nums[bingo_numbers[i]] = i;
    }
    
    size_t num_bingo_boards = 0;
    // find number of bingo boards which is equal to number of empty lines
    while((read = getline(&line, &len, file)) != -1) {
        if (strcmp(line, "\n") == 0) {
            num_bingo_boards++;
        }
    }
    
    // Allocate memory for an array containing all boards
    // One board is jsut an array with 25 elements
    // can use mod 5 to seperate rows and cols

    int **boards = (int**) malloc(sizeof(int*) * num_bingo_boards);
    if(boards == NULL) {
        printf("Out of memory on boards\n");
        exit(1);
    }

    // Allocate memory for every single board
    
    for (i = 0; i < num_bingo_boards; i++) {
        boards[i] = (int *) malloc(sizeof(int) * BOARD_SIZE);
        if (boards[i] == NULL) {
            printf("Out of memory on boards[%zu]\n", i);
            exit(1);
        }
    }
    

    // Rewing file and start reading in bingo boards
    rewind(file);
    // Skip first bingo numbers
    read = getline(&line, &len, file);
    int board_index = -1;
    size_t num_index = 0;
    
    while ((read = getline(&line, &len, file)) != -1) {
        // if empty line start with new board
        if (strcmp(line, "\n") == 0) {
            board_index++;
            num_index = 0;
            continue;
        }
        
        // Every line has 5 numbers seperated by a whitespace
        
        char *token = strtok(line, " ");
        while (token != NULL) {
            boards[board_index][num_index++] = atoi(token);
            token = strtok(NULL, " ");
            
        } 
    }

    // Loop thorugh every board
    
    int pulls_needed_to_win;
    int row_or_col;
    int right_col_or_row_index;
    int right_board_index;
    int most_pulls_needed_to_win = -1;
    
    int final_row_or_col;
    int final_col_or_row_index;
    int final_board_index;


    for(i = 0; i < num_bingo_boards; i++) {
        int pulls_needed_to_win = 9999999;
        int row_or_col = -1;
        int right_col_or_row_index = -1;
        int right_board_index = -1;
        // check every row
        for (int j = 0; j < ROW_SIZE; j++) {
            // Build row
            int row[ROW_SIZE];
            for (int e = 0; e < ROW_SIZE; e++) {
                row[e] = boards[i][(ROW_SIZE * j) + e];
            }
            int ret = count_nums_for_bingo(row, cached_bingo_nums, len_cached_bingo_nums);
            if (ret < pulls_needed_to_win) {
                pulls_needed_to_win = ret;
                row_or_col = ROW;
                right_col_or_row_index = j;
                right_board_index = i; 
            }

            // Build col
            for (int e = 0; e < ROW_SIZE; e++) {
                row[e] = boards[i][j + (ROW_SIZE * e)];
            }
            ret = count_nums_for_bingo(row, cached_bingo_nums, len_cached_bingo_nums);
            if (ret < pulls_needed_to_win) {
                pulls_needed_to_win = ret;
                row_or_col = COL;
                right_col_or_row_index = j; 
                right_board_index = i;
            }    
        }
        // Last board to win is the one with the smallest min
        if (most_pulls_needed_to_win < pulls_needed_to_win) {
            most_pulls_needed_to_win = pulls_needed_to_win;
            final_row_or_col = row_or_col;
            final_col_or_row_index = right_col_or_row_index;
            final_board_index = right_board_index;
        }
    }

    int last_bingo_num_index = -1;
    int last_bingo_num = -1;
    // Find which number was last in the row or column
    if (final_row_or_col == ROW) {
        for(i = 0; i < ROW_SIZE; i++) {
            int curr = boards[final_board_index][(ROW_SIZE * final_col_or_row_index) + i];
            // If the number is bigger it came later 
            if (cached_bingo_nums[curr] > last_bingo_num_index) {
                last_bingo_num_index = cached_bingo_nums[curr];
                last_bingo_num = curr;
            }
        }


    } else {
        for(i = 0; i < ROW_SIZE; i++) {
            int curr = boards[final_board_index][final_col_or_row_index + (i * ROW_SIZE)];
            // If the number is bigger it came later 
            if (cached_bingo_nums[curr] > last_bingo_num_index) {
                last_bingo_num_index = cached_bingo_nums[curr];
                last_bingo_num = curr;
            }
        }
    }


    // Find which numbers are not marked
    // if cached_bingo_nums at that number is less than cached bingo nums at the last number then it is marked
    int sum = 0;
    for(i = 0; i < ROW_SIZE * ROW_SIZE; i++) {
        int curr = boards[final_board_index][i];
        if(cached_bingo_nums[curr] > cached_bingo_nums[last_bingo_num]) {
            sum += curr;
        }
    }
    
    printf("Answer: %d\n", sum * last_bingo_num);
    
    for(i = 0; i < num_bingo_boards; i++) {
        free(boards[i]);
    }
    free(boards);
    free(bingo_numbers);
    free(cached_bingo_nums);
    return 0;
}
