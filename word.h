/*

    Simple Stemmer
    Version 1.0

    Header word.h
    for detailed description see word.c

    Dialect : ANSI C
    Compiler: any ANSI C-compatible

    Copyright (c) Martin Bruna, 2019
    Seminar work of 'KIV/PC'

*/

#include <stdlib.h>
#include <string.h>
#include "trie.h"

#ifndef STEMER_ALGORITHM_H
#define STEMER_ALGORITHM_H

/* ____________________________________________________________________________

                FUNCTION PROTOTYPES
   ____________________________________________________________________________
*/

void word_get_all_lcs(char *first, char *second, trie *trie, int msl);
int word_char_to_lower(int c);
int word_get_lowercase_alphabet_char(int c);

#endif
