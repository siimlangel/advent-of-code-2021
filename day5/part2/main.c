#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#define DIAGONAL 0
#define HORIZONTAL 1
#define VERTICAL 2


typedef struct Line {
    int x1;
    int y1;
    int x2;
    int y2;
    int heading;
} Line;

void Line_print(Line *line)
{
    printf("%d (%d, %d) (%d, %d)\n",line->heading, line->x1, line->y1, line->x2, line->y2);
}

// Naming horizontal or vertical "straight" here for a lack of a better word
bool Line_is_straight(Line *line)
{
    return (line->heading != DIAGONAL);
}

// Set min_ptr to min of two choices and max_ptr to max
void min_max(int choice_1, int choice_2, int *min_ptr, int *max_ptr)
{
    *min_ptr = choice_1 * (choice_1 < choice_2) + choice_2 * (choice_2 < choice_1);
    *max_ptr = choice_1 * (choice_1 > choice_2) + choice_2 * (choice_2 > choice_1);
}

// Add point and return 1 if it has not been intersected yet
int add_point(int *points, int x_coord, int y_coord, int row_len)
{
    int index = x_coord + (y_coord * row_len);
    int ret = 0;
    if (points[index] == 1) ret = 1;

    points[index] += 1;

    return ret;
}

int main(int argc, char **argv)
{
    
    if (argc != 2) {
        printf("Usage: %s [input_file_name.ext]\n", argv[0]);
    }

    char *FILENAME = *(argv + 1);
    FILE *file;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    
    file = fopen(FILENAME, "r");
    assert(file != NULL);
    
    // Find number of lines (number of points) in file
    size_t len_lines = 0;
    while((read = getline(&line, &len, file)) != -1) {
        len_lines++;    
    }

    rewind(file);
    // Allocate memory for the arr of lines and every line
    Line **lines = malloc(sizeof(Line*) * len_lines);
    size_t i = 0;
    for(i = 0; i < len_lines; i++) {
        lines[i] = malloc(sizeof(Line));
    }
    
    i = 0;
    int x1,x2,y1,y2;

    int max = -1;
    // Read all lines into an array and find the min and max values
    while((read = getline(&line, &len, file)) != -1) {
        char *line1 = strtok(line, " -> ");
        char *line2 = strtok(NULL, " -> ");
        
        x1 = atoi(strtok(line1, ","));
        y1 = atoi(strtok(NULL, ","));
        x2 = atoi(strtok(line2, ","));
        y2 = atoi(strtok(NULL, ","));

        if (x1 > max) max = x1;
        if (x2 > max) max = x2;
        if (y1 > max) max = y1;
        if (y2 > max) max = y2;

        lines[i]->x1 = x1;
        lines[i]->x2 = x2;
        lines[i]->y1 = y1;
        lines[i]->y2 = y2;
        if (lines[i]->x1 == lines[i]->x2) {
            lines[i]->heading = VERTICAL;
        } else if (lines[i]->y1 == lines[i]->y2) {
            lines[i]->heading = HORIZONTAL;
        } else {
            lines[i]->heading = DIAGONAL;
        }
        
        i++;
    }  

    int row_len = max + 1;

    int *points = calloc(row_len * row_len, sizeof(int));
    
    // Find all points a line passes through and mark them in points
    int count_intersecting_points = 0;
    for(i = 0; i < len_lines; i++) {
        int x_coord, y_coord, start, end;
        // If line is vertical
        if (lines[i]->heading == VERTICAL) {
            // Get start and end of a line
            min_max(lines[i]->y1, lines[i]->y2, &start, &end);

            // For vertical lines x_coord stays the same
            x_coord = lines[i]->x1;
            for(int j = start; j <= end; j++) {
                y_coord = j;
                count_intersecting_points += add_point(points, x_coord, y_coord, row_len);
            }
            
        }
        // If line is horizontal
        if(lines[i]->heading == HORIZONTAL) {
            min_max(lines[i]->x1,lines[i]->x2, &start, &end);
            y_coord = lines[i]->y1;
            for(int j = start; j <= end; j++) {
                x_coord = j;
                count_intersecting_points += add_point(points, x_coord, y_coord, row_len);
            }
        
        }
        // Could be moving +x -y, -x +y, +x +y or -x -y
        int diag_x_heading = 1; 
        int diag_y_heading = 1;
        int line_len;
        if (lines[i]->heading == DIAGONAL) {
            if (lines[i]->x1 > lines[i]->x2) diag_x_heading = -1;
            if (lines[i]->y1 > lines[i]->y2) diag_y_heading = -1;

            // Lines are 45 so one triangle side gets us length
            line_len = abs(lines[i]->x1 - lines[i]->x2);
            
            // Iterate length of line + 1 for the endpoint
            for (int j = 0; j < line_len + 1; j++) {
                x_coord = lines[i]->x1 + (j * diag_x_heading);
                y_coord = lines[i]->y1 + (j * diag_y_heading);
                count_intersecting_points += add_point(points, x_coord, y_coord, row_len);
            }
        }
        
    }


    printf("%d\n", count_intersecting_points);
    for(i = 0; i < len_lines; i++) {
        free(lines[i]);
    }
    free(lines);
    free(points);
    if(line) free(line);
    fclose(file);
    return 0;
}
