#include <stdio.h>
#include <stdlib.h>
#include "trie.h"

#ifndef STEMER_STEMING_H
#define STEMER_STEMING_H

void find_stem(struct trie * stems, const char * word);

void find_stems(int msf, const char * words);

struct trie * load_stems(int msf);

void create_stems(FILE * input_file, int msl);

trie * create_words(FILE * input_file);

trie * create_stems_from_words(trie * words, int msl);

void write_stems_to_file(trie * stems);

#endif
