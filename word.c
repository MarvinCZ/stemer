/*

    Simple Stemmer
    Version 1.0

    Module word.c
    Module with helper function around strings

    Dialect : ANSI C
    Compiler: any ANSI C-compatible

    Copyright (c) Martin Bruna, 2019
    Seminar work of 'KIV/PC'

*/

#include <ctype.h>
#include <stdio.h>
#include "word.h"
#include "trie.h"

/* ____________________________________________________________________________

    void word_get_all_lcs(char * first, char * second, trie * trie, int msl)

    first - pointer to first word
    second - pointer to first word
    trie - pointer to trie to which will be sequences saved
    msl - minimum stem length - if length of stem is less than msl it will be ignored

    finds all subsequences between first and second word and inserts them into trie
   ____________________________________________________________________________
*/
void word_get_all_lcs(char *first, char *second, trie *trie, int msl) {
    size_t m = strlen(first);
    size_t n = strlen(second);
    int i, j;
    int **matrix;
    int len = 0;
    char temp = 0;
    int temp_len = 0;

    /* dynamic allocation of n*m matrix */
    matrix = malloc(sizeof(int*) * (m+1));
    for (i = 0; i < m+1; i++) {
        matrix[i] = calloc(1, sizeof(int) * (n+1));
    }

    /* build LCS matrix */
    for (i = 0; i <= m; i++) {
        for (j = 0; j <= n; j++) {
            if (i == 0 || j == 0) /* first row and column is always 0*/
                matrix[i][j] = 0;

            else if (first[i - 1] == second[j - 1]) { /* if words has same char at given cords we can add 1 to value before*/
                matrix[i][j] = matrix[i - 1][j - 1] + 1;
                if (len < matrix[i][j]) { /* keep length of longest subsequence not much needed (just check if any subsequence) */
                    len = matrix[i][j];
                }
            }
            else
                matrix[i][j] = 0;
        }
    }

    /* iterate over while matrix and find ends of subsequence */
    if (len != 0) {
        for (i = 0; i <= m; i++) {
            for (j = 0; j <= n; j++) {
                if (matrix[i][j] >= msl) {
                    if ((i < m && j < n) && matrix[i + 1][j + 1] != 0) { /* if not on edge and i+1 && j+1 is not 0 it can not be end of subsequence */
                        continue;
                    }
                    temp_len = matrix[i][j];
                    temp = first[i];
                    first[i] = '\0'; /* mark as end of the word */
                    trie_add(trie, &first[i-temp_len]);
                    first[i] = temp; /* unmark end of the word */
                }
            }
        }
    }


    for (i = 0; i < m+1; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

/* ____________________________________________________________________________

    int word_char_to_lower(int c)

    c - input char

    converts uppercase or lowercase letter to lowercase
   ____________________________________________________________________________
*/
int word_char_to_lower(int c) {
    if (c >= 'A' && c <= 'Z') {
        return tolower(c);
    }

    if (c >= 0xC0 && c <= 0xDE) { /* Windows-1250 + ISO_8859-2 */
        return c + 0x20;
    }

    if (c >= 0x8A && c <= 0x8F && c != 0x8B) { /* Windows-1250 */
        return c + 0x10;
    }

    if (c == 0xA9 || c == 0xAE) { /* ISO_8859-2 */
        return c + 0x10;
    }


    return c;
}

/* ____________________________________________________________________________

    int word_get_lowercase_alphabet_char(int c)

    c - input char

    returns lowercase variant of letter if not letter returns -1
   ____________________________________________________________________________
*/
int word_get_lowercase_alphabet_char(int c) {
    int output = word_char_to_lower(c);
    if (output >= 0x61 && output <= 0x7A) { /* global */
        return output;
    }
    if (output >= 0x9A && output <= 0x9F && output != 0x9B) { /* Windows-1250 */
        return output;
    }
    if (output >= 0xE0 && output <= 0xFE) { /* Windows-1250 + ISO_8859-2 */
        return output;
    }
    if (output == 0xB9 || output == 0xBE) { /* ISO_8859-2 */
        return output;
    }
    return -1;
}
