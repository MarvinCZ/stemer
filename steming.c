#include <stdio.h>
#include <stdlib.h>
#include "steming.h"
#include "trie.h"
#include "bool.h"
#include "algorithm.h"

static unsigned char word_stops[] = {'.', ',', '\'', '"', ' ', '!', '?', '\n', '\r', '\t', ':', '-',';',11,12,133,160, '(', ')', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

void find_stem(struct trie * stems, const char * word) {
    char * out;

    out = trie_common_sub_sequence(stems, word);
    if (out[0] == '\0') {
        out[0] = '0';
        out[1] = '\0';
    }
    printf("%s -> %s\r\n", word, out);
    free(out);
}

void find_stems(int msf, const char * words) {
    char buffer[50] = "";
    int i = 0;
    int j = 0;
    trie * stems = load_stems(msf);

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

    trie_free(stems);
}

struct trie * load_stems(int msf) {
    FILE * input_file;
    trie * stems = trie_create_node(NULL, '\0');
    char line[100];
    int i;
    int text = 0;
    int count;
    int c;
    char ch;

    input_file = fopen("stems.dat", "r");
    while ((c = fgetc(input_file)) != EOF) {
        ch = (char)c;
        line[text++] = ch;
        if (ch == '\n') {
            line[text] = '\0';
            text = 0;
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
    }

    fclose(input_file);
    return stems;
}

void create_stems(FILE * input_file, int msl) {
    trie * stems;
    trie * trie;

    trie = create_words(input_file);
    stems = create_stems_from_words(trie, msl);
    trie_free(trie);
    write_stems_to_file(stems);
    trie_free(stems);
}

trie * create_words(FILE * input_file) {
    int c;
    int i;
    bool stop;
    char buffer[50] = "";
    int buffer_pointer = 0;
    trie * trie = trie_create_node(NULL, '\0');

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
            buffer[buffer_pointer++] = (char)char_to_lower(c);
        }
    }
    fclose(input_file);
    buffer[buffer_pointer] = '\0';
    trie_add(trie, buffer);

    return trie;
}

trie * create_stems_from_words(trie * words, int msl) {
    struct trie_iterator * iterator;
    struct trie_iterator * inner_iterator;
    char buffer[50] = "";
    char inner_buffer[50] = "";
    int count = 0;
    trie * stems = trie_create_node(NULL, '\0');

    iterator = trie_get_iterator(words);
    while ((trie_iterator_get_next(iterator, buffer)) > 0) {
        count++;
        inner_iterator = trie_clone_iterator(iterator);
        while ((trie_iterator_get_next(inner_iterator, inner_buffer)) > 0) {
            lcs(buffer, inner_buffer, stems, msl);
        }
        free(inner_iterator);
    }
    free(iterator);

    return stems;
}

void write_stems_to_file(trie * stems) {
    FILE * output_file;
    char buffer[50] = "";
    int result = 0;
    struct trie_iterator * iterator;

    output_file = fopen("stems.dat", "w");
    iterator = trie_get_iterator(stems);
    while ((result = trie_iterator_get_next(iterator, buffer)) > 0) {
        fprintf(output_file, "%s %d\r\n", buffer, result);
    }
    fclose(output_file);
    free(iterator);
}
