#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "bool.h"
#include "trie.h"

void trie_print(trie * trie, char * buffer, int index) {
    int i;
    buffer[index] = trie->letter;
    buffer[index+1] = '\0';
    if (trie->count > 0) {
        printf("%s\n", buffer);
    }
    for (i = 0; i < 256; i++) {
        if (trie->childs[i] != NULL) {
            trie_print(trie->childs[i], buffer, index+1);
        }
    }
    buffer[index] = '\0';
}


trie *trie_create_node(trie *parent, char letter) {
    int i;
    trie * node = malloc(sizeof(trie));
    node->count = 0;
    node->parent = parent;
    node->letter = (unsigned char) letter;
    for (i = 0; i < 256; i++) {
        node->childs[i] = NULL;
    }
    if (parent != NULL) {
        parent->childs[node->letter] = node;
    }
    return node;
}

void trie_add(trie *root, char *word) {
    trie * node = root;
    char * que = word;

    while (*que != '\0') {
        if (node->childs[(unsigned char)*que] == NULL) {
            node->childs[(unsigned char)*que] = trie_create_node(node, *que);
        }

        node = node->childs[(unsigned char)*que];
        que++;
    }

    node->count++;
}

trie_iterator *trie_get_iterator(trie *root) {
    trie_iterator * iterator = malloc(sizeof(trie_iterator));
    iterator->accual = root;
    iterator->back_from = '\0';
    iterator->before = NULL;
    iterator->done = false;
    return iterator;
}

trie * trie_get_child(trie * node, unsigned char min_index) {
    int i;

    for (i = min_index + 1; i < 256; i++) {
        if (node->childs[i] != NULL) {
            return node->childs[i];
        }
    }

    return NULL;
}

void trie_create_word(trie *node, char *output) {
    int length = 0;
    int i, j;
    char temp;

    while (node != NULL) {
        output[length++] = node->letter;
        node = node->parent;
    }
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

int trie_iterator_get_next(trie_iterator *iterator, char *output) {
    trie * node = iterator->accual;
    trie * temp = NULL;

    if (iterator->done == true) {
        return -1;
    }

    while (node->count <= 0 || node == iterator->before) {
        temp = trie_get_child(node, iterator->back_from);
        if (temp == NULL) {
            iterator->back_from = node->letter;
            if (node->parent == NULL) {
                iterator->done = true;
                return -1;
            }
            node = node->parent;
            iterator->before = node;
        } else {
            iterator->back_from = '\0';
            iterator->before = node;
            node = temp;
        }
    }

    trie_create_word(node, output);
    iterator->accual = node;
    iterator->before = node;
    return node->count;

}

void trie_free(trie *root) {
    int i;
    for (i = 0; i < 256; i++) {
        if (root->childs[i] != NULL) {
            trie_free(root->childs[i]);
        }
    }
    free(root);
}

trie_iterator *trie_clone_iterator(trie_iterator *iterator) {
    trie_iterator * new_iterator = malloc(sizeof(trie_iterator));
    new_iterator->accual = iterator->accual;
    new_iterator->back_from = iterator->back_from;
    new_iterator->before = iterator->before;
    new_iterator->done = iterator->done;
    return new_iterator;
}

char * trie_common_sub_sequence(trie *root, const char *word) {
    int max_length = 0;
    int i;
    int j;
    char * buffer = malloc(sizeof(char) * 50);
    trie * node;
    size_t len = strlen(word);

    buffer[0] = '\0';

    for (i = 0; i < len; i++) {
        j = 0;
        node = root;
        while (i + j < len) {
            node = node->childs[(unsigned char)word[i+j]];
            if (node == NULL) {
                break;
            }
            if (j > max_length && node->count > 0) {
                trie_create_word(node, buffer);
                max_length = j;
            }
            j++;
        }
    }

    return buffer;
}


