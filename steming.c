/*

    Simple Stemmer
    Version 1.0

    Module steming.c
    Module for creating stems from words and finding stems in words.

    Dialect : ANSI C
    Compiler: any ANSI C-compatible

    Copyright (c) Martin Bruna, 2019
    Seminar work of 'KIV/PC'

*/

#include <stdio.h>
#include <stdlib.h>
#include "steming.h"
#include "trie.h"
#include "word.h"
#include "define.h"

/* ____________________________________________________________________________

    void steaming_find_stem(struct trie *stems, const char *word)

    stems - pointer to trie containing all stems
    word - pointer to array containing word

    Finds longest stem for given word and prints it to STDOUT
   ____________________________________________________________________________
*/
void steaming_find_stem(struct trie *stems, const char *word) {
    size_t size = 0;
    size_t max_size = 0;
    size_t temp_size = 0;
    char * temp;
    char * out;
    int i = 0;

    out = calloc(1, sizeof(char));

    size = strlen(word);

    for (i = 0; i < size; i++) {
        temp = trie_common_prefix(stems, &word[i]);
        temp_size = strlen(temp);
        /* Longer stem is always better if sizes are same ANSCI order is taken as rule */
        if (temp_size > max_size || (temp_size == max_size && strcmp(temp, out) < 0)) {
            free(out);
            out = temp;
            max_size = temp_size;
        } else {
            free(temp);
        }
    }

    if (max_size > 0) {
        printf("%s -> %s\r\n", word, out);
    } else {
        printf("%s -> 0\r\n", word);
    }

    free(out);
}

/* ____________________________________________________________________________

    void steaming_find_stems(int msf, const char *words)

    msf - minimum stem frequency
    words - pointer to array containing word sequence

    Finds longest stems for given words and prints them to STDOUT
   ____________________________________________________________________________
*/
int steaming_find_stems(int msf, const char *words) {
    char buffer[MAX_WORD_LENGTH] = "";
    int i = 0;
    int j = 0;
    trie * stems = steaming_load(msf);
    int c;

    if (stems == NULL) {
        return -1;
    }

    while (true) {
        c = word_get_lowercase_alphabet_char((unsigned char) words[j]);

        if (c == -1) {
            buffer[i] = '\0';
            i = 0;
            steaming_find_stem(stems, buffer);
            if (words[j] == '\0') { /* end of args */
                break;
            }
        } else {
            buffer[i++] = (char)c;
        }

        j++;
    }

    trie_free(stems);
    return 0;
}

/* ____________________________________________________________________________

    struct trie * steaming_load(int msf)

    msf - minimum stem frequency

    Loads steams from file "stems.dat", return pointer to struct trie on success and NULL when failed
   ____________________________________________________________________________
*/
struct trie * steaming_load(int msf) {
    FILE * input_file;
    trie * stems;
    char line[100];
    int i;
    int text = 0;
    int count;
    int c;
    char ch;

    input_file = fopen("stems.dat", "r");

    if (input_file == NULL) {
        return NULL;
    }

    stems = trie_create_node(NULL, '\0');
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

/* ____________________________________________________________________________

    void steaming_create(FILE *input_file, int msl)

    input_file - pointer to opened file
    msl - minimum stem length - stem with length lower than msl will be ignored

    Loads words from file, creates stems from words and saves them to file.
   ____________________________________________________________________________
*/
void steaming_create(FILE *input_file, int msl) {
    trie * stems;
    trie * trie;

    trie = steaming_create_words(input_file);
    stems = steaming_create_from_words(trie, msl);
    trie_free(trie);
    steaming_write_to_file(stems);
    trie_free(stems);
}

/* ____________________________________________________________________________

    void steaming_create_words(FILE *input_file)

    input_file - pointer to opened file

    Loads words from file.
    Returns trie with words.
   ____________________________________________________________________________
*/
trie * steaming_create_words(FILE *input_file) {
    int c;
    bool stop;
    char buffer[MAX_WORD_LENGTH] = "";
    int buffer_pointer = 0;
    trie * trie = trie_create_node(NULL, '\0');


    while ((c = fgetc(input_file)) != EOF) {
        c = word_get_lowercase_alphabet_char(c);
        stop = c == -1;

        if (stop) { /* if char is word break than save word to trie otherwise expand word with char */
            buffer[buffer_pointer] = '\0';
            buffer_pointer = 0;
            trie_add(trie, buffer);
        } else {
            buffer[buffer_pointer++] = (char)c;
        }
    }
    fclose(input_file);
    /* add last unfinished word */
    buffer[buffer_pointer] = '\0';
    trie_add(trie, buffer);

    return trie;
}

/* ____________________________________________________________________________

    trie * steaming_create_from_words(trie * words, int msl)

    words - pointer to trie containing words
    msl - minimum stem length - stem with length lower than msl will be ignored

    iterate over words and create all possible stems

    returns pointer to trie containing stems
   ____________________________________________________________________________
*/
trie * steaming_create_from_words(trie * words, int msl) {
    struct trie_iterator * iterator;
    struct trie_iterator * inner_iterator;
    char buffer[MAX_WORD_LENGTH] = "";
    char inner_buffer[MAX_WORD_LENGTH] = "";
    int count = 0;
    trie * stems = trie_create_node(NULL, '\0');

    iterator = trie_get_iterator(words);
    /* outer iteration - word is in buffer */
    while ((trie_iterator_get_next(iterator, buffer)) > 0) {
        count++;
        inner_iterator = trie_clone_iterator(iterator);
        /* inner iteration (starting next word after outer iteration) - word is in inner_buffer */
        while ((trie_iterator_get_next(inner_iterator, inner_buffer)) > 0) {
            word_get_all_lcs(buffer, inner_buffer, stems, msl);
        }
        free(inner_iterator);
    }
    free(iterator);

    return stems;
}

/* ____________________________________________________________________________

    void steaming_write_to_file(trie * stems)

    stems - pointer to trie containing stems

    iterate over words and create all possible stems

    returns pointer to trie containing stems
   ____________________________________________________________________________
*/
void steaming_write_to_file(trie * stems) {
    FILE * output_file;
    char buffer[MAX_WORD_LENGTH] = "";
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
