#include <stdio.h>
#include <stdlib.h>
#include <string.h>




void dump(int arr[9][9])
{
    size_t i, j;
    for(i = 0; i < 9; i++) {
        for(j = 0; j < 9; j++) {
            printf("%d ", arr[i][j]);
        }
        printf("\n");
    }
}

void modify(int arr[9][9])
{
    size_t i, j;
    for(i = 0; i < 9; i++) {
        for(j = 0; j < 9; j++) {
            arr[i][j] = i + j;
        }
    }
    
}

int main(int argc, char **argv)
{
    
    int arr[9][9];
    size_t i;
    for(i = 0; i < 9; i++) {
        memset(arr[i], 0, sizeof(int) * 9);
    }

    modify(arr);
    dump(arr);

    return 1;
}
