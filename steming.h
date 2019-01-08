/*

    Simple Stemmer
    Version 1.0

    Header steming.h
    for detailed description see steaming.c

    Dialect : ANSI C
    Compiler: any ANSI C-compatible

    Copyright (c) Martin Bruna, 2019
    Seminar work of 'KIV/PC'

*/


#include <stdio.h>
#include <stdlib.h>
#include "trie.h"

#ifndef STEMER_STEMING_H
#define STEMER_STEMING_H

/* ____________________________________________________________________________

                FUNCTION PROTOTYPES
   ____________________________________________________________________________
*/

void steaming_find_stem(struct trie *stems, const char *word);
int steaming_find_stems(int msf, const char *words);
struct trie * steaming_load(int msf);
void steaming_create(FILE *input_file, int msl);
trie * steaming_create_words(FILE *input_file);
trie * steaming_create_from_words(trie *words, int msl);
void steaming_write_to_file(trie *stems);

#endif
