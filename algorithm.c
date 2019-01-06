//
// Created by marvin on 1/4/19.
//
#include "algorithm.h"

char * lcs(const char * first, const char * second) {
    char * buffer;
    size_t m = strlen(first);
    size_t n = strlen(second);
    int i, j;

    int LCSuff[m + 1][n + 1];

    int len = 0;

    int row = 0;
    int col = 0;

    for (i = 0; i <= m; i++) {
        for (j = 0; j <= n; j++) {
            if (i == 0 || j == 0)
                LCSuff[i][j] = 0;

            else if (first[i - 1] == second[j - 1]) {
                LCSuff[i][j] = LCSuff[i - 1][j - 1] + 1;
                if (len < LCSuff[i][j]) {
                    len = LCSuff[i][j];
                    row = i;
                    col = j;
                }
            }
            else
                LCSuff[i][j] = 0;
        }
    }

    // if true, then no common substring exists
    if (len == 0) {
        buffer = malloc(sizeof(char));
        buffer[0] = '\0';
        return buffer;
    }

    // allocate space for the longest common substring
    buffer = (char*)calloc(1, (len + 1) * sizeof(char));

    // traverse up diagonally form the (row, col) cell
    // until LCSuff[row][col] != 0
    while (LCSuff[row][col] != 0) {
        buffer[--len] = first[row - 1]; // or Y[col-1]

        // move diagonally up to previous cell
        row--;
        col--;
    }

    // required longest common substring
    return buffer;
}
