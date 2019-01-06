//
// Created by marvin on 1/3/19.
//
#include "bool.h"

#ifndef STEMER_TRIE_H
#define STEMER_TRIE_H

typedef struct trie trie;

struct trie {
    trie * parent;
    int count;
    unsigned char letter;
    trie * childs[256];
};

typedef struct trie_iterator {
    trie * accual;
    unsigned char back_from;
    trie * before;
    bool done;
} trie_iterator;

void trie_print(trie * trie, char * buffer, int index);

trie* trie_create_node(trie * parent, char letter);

void trie_add(trie * root, char * word);

bool trie_contains(trie * root, char * word);

trie_iterator * trie_get_iterator(trie * root);

trie_iterator * trie_clone_iterator(trie_iterator * iterator);

int trie_iterator_get_next(trie_iterator * iterator, char * output);

void trie_free(trie * root);

#endif //STEMER_TRIE_H
