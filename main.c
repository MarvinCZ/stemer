#include <stdio.h>
#include <stdlib.h>
#include "trie.h"
#include "bool.h"
#include "algorithm.h"

static unsigned char word_stops[] = {'.', ',', '\'', '"', ' ', '!', '?', '\n', '\r', '\t', ':', '-',';',11,12,133,160, '(', ')', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

void find_stem(trie * stems, const char * word) {

}

void find_stems(trie * stems, const char * words) {
    char buffer[50] = "";
    int i = 0;
    int j = 0;

    while (true) {
        if (words[j] == ' ' || words[j] == '\0') {
            buffer[i] = '\0';
            i = 0;
            find_stem(stems, buffer);
            if (words[j++] == '\0') {
                break;
            }
        }

        buffer[i++] = words[j++];
    }
}

trie * load_stems(int msf) {
    FILE * input_file;
    trie * stems = trie_create_node(NULL, '\0');
    char * line;
    size_t len = 0;
    int i;
    int count;

    input_file = fopen("stems.dat", "r");
    while (getline(&line, &len, input_file) != -1) {
        i = 0;
        while (line[i] != ' ') {
            i++;
        }
        line[i] = '\0';
        count = atoi(&line[i+1]);
        if (msf <= count) {
            trie_add(stems, line);
        }
    }

    free(line);
    fclose(input_file);
    return stems;
}

void create_stems(FILE * input_file, int msl, int msf) {
    FILE * output_file;
    char * buffer = malloc(sizeof(char) * 50);
    size_t buffer_pointer = 0;
    int c;
    int i;
    char inner_buffer[50];
    char * longestsubsequence;
    int result = 0;
    int inner_result = 0;
    int count = 0;
    bool stop;
    trie_iterator * iterator;
    trie_iterator * inner_iterator;
    trie * stems;
    trie * trie = trie_create_node(NULL, '\0');
    stems = trie_create_node(NULL, '\0');

    while ((c = fgetc(input_file)) != EOF) {
        stop = false;
        for (i = 0; i < 29; i++) {
            if (c == word_stops[i]) {
                stop = true;
                break;
            }
        }

        if (stop) {
            buffer[buffer_pointer] = '\0';
            buffer_pointer = 0;
            trie_add(trie, buffer);
        } else {
            buffer[buffer_pointer++] = (char)c;
        }
    }
    fclose(input_file);
    buffer[buffer_pointer] = '\0';
    trie_add(trie, buffer);

    iterator = trie_get_iterator(trie);
    while ((trie_iterator_get_next(iterator, buffer)) > 0) {
        count++;
        inner_iterator = trie_clone_iterator(iterator);
        while ((trie_iterator_get_next(inner_iterator, inner_buffer)) > 0) {
            longestsubsequence = lcs(buffer, inner_buffer);
            if (strlen(longestsubsequence) >= msl) {
                trie_add(stems, longestsubsequence);
            }
            free(longestsubsequence);
        }
        free(inner_iterator);
    }
    free(iterator);

    trie_free(trie);

    output_file = fopen("stems.dat", "w");
    iterator = trie_get_iterator(stems);
    while ((result = trie_iterator_get_next(iterator, buffer)) > 0) {
        fprintf(output_file, "%s %d\r\n", buffer, result);
    }
    fclose(output_file);
    free(iterator);
    trie_free(stems);
    free(buffer);
}

int main(int argc, char **argv) {
    FILE * file;
    char * filename = malloc(sizeof(char) * (strlen(argv[1])+5));
    trie * stems;

    strcpy(filename, argv[1]);
    file = fopen(filename, "r");
    if (file == NULL) {
        strcat(filename, ".txt");
        file = fopen(filename, "r");
    }
    free(filename);

    if (file == NULL) {
        stems = load_stems(10);
        find_stems(stems, argv[1]);
        trie_free(stems);
    } else {
        create_stems(file, 3, 10);
    }

    return 0;
}