/*

    Simple Stemmer
    Version 1.0

    Header trie.h
    for detailed description see trie.c

    Dialect : ANSI C
    Compiler: any ANSI C-compatible

    Copyright (c) Martin Bruna, 2019
    Seminar work of 'KIV/PC'

*/

#include "define.h"

#ifndef STEMER_TRIE_H
#define STEMER_TRIE_H

/* ____________________________________________________________________________

                STRUCTURES
   ____________________________________________________________________________
*/
typedef struct trie trie;

/* ____________________________________________________________________________

    structure representing node of trie
   ____________________________________________________________________________
*/
struct trie {
    trie * parent;
    int count;
    unsigned char letter;
    trie * childs[MAX_CHAR_INDEX];
};

/* ____________________________________________________________________________

    structure representing state of iteration
   ____________________________________________________________________________
*/

typedef struct trie_iterator {
    trie * accual;
    unsigned char back_from;
    trie * before;
    bool done;
} trie_iterator;

/* ____________________________________________________________________________

                FUNCTION PROTOTYPES
   ____________________________________________________________________________
*/

trie* trie_create_node(trie * parent, char letter);
void trie_add(trie * root, char * word);
char * trie_common_prefix(trie *root, const char *word);
trie_iterator * trie_get_iterator(trie * root);
trie_iterator * trie_clone_iterator(trie_iterator * iterator);
int trie_iterator_get_next(trie_iterator * iterator, char * output);
void trie_free(trie * root);

#endif
