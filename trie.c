/*

    Simple Stemmer
    Version 1.0

    Module trie.c
    Module for trie. Module contains methods for work with trie.

    Dialect : ANSI C
    Compiler: any ANSI C-compatible

    Copyright (c) Martin Bruna, 2019
    Seminar work of 'KIV/PC'

*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "trie.h"
#include "word.h"
#include "define.h"

/* ____________________________________________________________________________

    trie * trie_create_node(trie * parent, char letter)

    parent - parent node for letter
    letter - char of letter

    creates node and registers it to its parent
   ____________________________________________________________________________
*/
trie *trie_create_node(trie *parent, char letter) {
    int i;
    trie * node = malloc(sizeof(trie));
    node->count = 0;
    node->parent = parent;
    node->letter = (unsigned char) letter; /* some compilers can have char as -128 - 127 for indexing array we need unsigned */
    for (i = 0; i < MAX_CHAR_INDEX; i++) {
        node->childs[i] = NULL;
    }
    if (parent != NULL) {
        parent->childs[node->letter] = node;
    }
    return node;
}

/* ____________________________________________________________________________

    void trie_add(trie * root, char * word)

    root - root of trie
    word - pointer to char array containing word to be added to trie

    adds word to trie
   ____________________________________________________________________________
*/
void trie_add(trie * root, char * word) {
    trie * node = root;
    char * que = word;
    unsigned char c;

    /* iterates word to the end */
    while (*que != '\0') {
        c = (unsigned char)*que; /* some compilers can have char as -128 - 127 for indexing array we need unsigned */
        if (node->childs[c] == NULL) {
            node->childs[c] = trie_create_node(node, *que);
        }

        node = node->childs[c];
        que++;
    }

    /* mark as word end and also keep count of words */
    node->count++;
}

/* ____________________________________________________________________________

    trie_iterator *trie_get_iterator(trie *root)

    root - root of trie

    creates trie_iterator starting on begging
   ____________________________________________________________________________
*/
trie_iterator *trie_get_iterator(trie *root) {
    trie_iterator * iterator = malloc(sizeof(trie_iterator));
    iterator->accual = root;
    iterator->back_from = '\0';
    iterator->before = NULL;
    iterator->done = false;
    return iterator;
}

/* ____________________________________________________________________________

    trie * trie_get_child(trie * node, unsigned char min_index)

    node - origin node
    min_index - excluded minimum index of expected child

    returns next valid child node, NULL when such node does not exist
   ____________________________________________________________________________
*/
trie * trie_get_child(trie * node, unsigned char min_index) {
    int i;

    for (i = min_index + 1; i < MAX_CHAR_INDEX; i++) {
        if (node->childs[i] != NULL) {
            return node->childs[i];
        }
    }

    return NULL;
}

/* ____________________________________________________________________________

    void trie_create_word(trie * node, char * output)

    node - end node of word
    output - pointer to buffer to be filled

    fill buffer with word which ends in given node
   ____________________________________________________________________________
*/
void trie_create_word(trie * node, char * output) {
    int length = 0;
    int i, j;
    char temp;

    /* revers iteration over tree */
    while (node != NULL) {
        output[length++] = node->letter;
        node = node->parent;
    }

    /* word is in reverse order */
    length--;
    i = 0;
    j = length-1;
    while (j > i) {
        temp = output[i];
        output[i] = output[j];
        output[j] = temp;

        j--;
        i++;
    }
}

/* ____________________________________________________________________________

    int trie_iterator_get_next(trie_iterator * iterator, char * output)

    iterator - pointer to iteration state
    output - pointer to buffer to be filled with next word

    finds next word according to iteration state and fills output and
    returns number of word count
    on end returns -1
   ____________________________________________________________________________
*/
int trie_iterator_get_next(trie_iterator *iterator, char *output) {
    trie * node = iterator->accual;
    trie * temp = NULL;

    if (iterator->done == true) {
        return -1;
    }

    while (node->count <= 0 || node == iterator->before) {
        temp = trie_get_child(node, iterator->back_from); /* tries to get node on same level */
        if (temp == NULL) { /* theres no node on same level going back */
            iterator->back_from = node->letter; /* save accual letter so iterator knows where to start finding */
            if (node->parent == NULL) { /* no parent => end of iteration */
                iterator->done = true;
                return -1;
            }
            node = node->parent;
            iterator->before = node;
        } else { /* iteration to depth */
            iterator->back_from = '\0';
            iterator->before = node;
            node = temp;
        }
    }

    /* creates word from found node and fills output buffer */
    trie_create_word(node, output);
    iterator->accual = node;
    iterator->before = node;
    return node->count;

}

/* ____________________________________________________________________________

    void trie_free(trie * root)

    root - pointer to root node

    recursively frees trie
   ____________________________________________________________________________
*/
void trie_free(trie * root) {
    int i;
    for (i = 0; i < MAX_CHAR_INDEX; i++) {
        if (root->childs[i] != NULL) {
            trie_free(root->childs[i]);
        }
    }
    free(root);
}

/* ____________________________________________________________________________

    trie_iterator * trie_clone_iterator(trie_iterator * iterator)

    iterator - pointer to iterator

    creates clone of iterator
   ____________________________________________________________________________
*/
trie_iterator * trie_clone_iterator(trie_iterator * iterator) {
    trie_iterator * new_iterator = malloc(sizeof(trie_iterator));
    new_iterator->accual = iterator->accual;
    new_iterator->back_from = iterator->back_from;
    new_iterator->before = iterator->before;
    new_iterator->done = iterator->done;
    return new_iterator;
}

/* ____________________________________________________________________________

    char * trie_common_prefix(trie * root, const char * word)

    root - pointer to root of trie
    word - word with prefix to be found

    returns prefix of word which is also in trie
   ____________________________________________________________________________
*/
char * trie_common_prefix(trie *root, const char *word) {
    int max_length = 0;
    int i;
    char * buffer = malloc(sizeof(char) * MAX_WORD_LENGTH);
    trie * node;
    size_t len = strlen(word);

    buffer[0] = '\0';

    i = 0;
    node = root;
    while (i < len) {
        node = node->childs[(unsigned char)word[i]]; /* some compilers can have char as -128 - 127 for indexing array we need unsigned */
        if (node == NULL) { /* end of iteration */
            break;
        }
        if (i > max_length && node->count > 0) { /* new bigger prefix found */
            trie_create_word(node, buffer);
            max_length = i;
        }
        i++;
    }

    return buffer;
}


