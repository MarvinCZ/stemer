#include <ctype.h>
#include <stdio.h>
#include "algorithm.h"
#include "trie.h"

void lcs(char * first, char * second, trie * trie, int msl) {
    size_t m = strlen(first);
    size_t n = strlen(second);
    int i, j;
    int **LCSuff;
    int len = 0;
    char temp = 0;
    int temp_len = 0;

    LCSuff = malloc(sizeof(int*) * (m+1));
    for (i = 0; i < m+1; i++) {
        LCSuff[i] = calloc(1, sizeof(int) * (n+1));
    }

    for (i = 0; i <= m; i++) {
        for (j = 0; j <= n; j++) {
            if (i == 0 || j == 0)
                LCSuff[i][j] = 0;

            else if (first[i - 1] == second[j - 1]) {
                LCSuff[i][j] = LCSuff[i - 1][j - 1] + 1;
                if (len < LCSuff[i][j]) {
                    len = LCSuff[i][j];
                }
            }
            else
                LCSuff[i][j] = 0;
        }
    }

    if (len != 0) {
        for (i = 0; i <= m; i++) {
            for (j = 0; j <= n; j++) {
                if (LCSuff[i][j] >= msl) {
                    if ((i < m && j < n) && LCSuff[i + 1][j + 1] != 0) {
                        continue;
                    }
                    temp_len = LCSuff[i][j];
                    temp = first[i];
                    first[i] = '\0';
                    trie_add(trie, &first[i-temp_len]);
                    first[i] = temp;
                }
            }
        }
    }


    for (i = 0; i < m+1; i++) {
        free(LCSuff[i]);
    }
    free(LCSuff);
}

int char_to_lower(int c) {
    if (c >= 'A' && c <= 'Z') {
        return tolower(c);
    }

    if (c >= 0xC0 && c <= 0xDE) {
        return c + 0x20;
    }

    if (c >= 0x8A && c <= 0x8F) {
        if (c != 0x8B) return c + 0x10;
    }

    if (c == 0xAA || c == 0xAF) {
        return c + 0x10;
    }

    return c;
}
