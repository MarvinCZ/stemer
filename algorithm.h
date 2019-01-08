#ifndef STEMER_ALGORITHM_H
#define STEMER_ALGORITHM_H

#include <stdlib.h>
#include <string.h>
#include "trie.h"


void lcs(char * first, char * second, trie * trie, int msl);

int char_to_lower(int c);

#endif
