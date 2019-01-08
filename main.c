/*

    Simple Stemmer
    Version 1.0

    Module main.c
    This module composes all the parts of the stemmer together and
    contains the executable entry point function. It is also responsible
    for the user interaction.

    Dialect : ANSI C
    Compiler: any ANSI C-compatible

    Copyright (c) Martin Bruna, 2019
    Seminar work of 'KIV/PC'

    This file is provided "AS IS" with NO WARRANTY OF ANY KIND,
    INCLUDING THE WARRANTIES OF DESIGN, MERCHANTABILITY AND FITNESS FOR
    A PARTICULAR PURPOSE. Kamil Ekstein reserves all rights not expressly
    granted herein.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "define.h"
#include "steming.h"
#include "word.h"

/* ____________________________________________________________________________

    void load_optional_arguments()

    Parses optional MSL and MSF arguments
   ____________________________________________________________________________
*/
void load_optional_arguments(int argc, char** argv, int * msl, int * msf) {
    int i;
    for (i = 2; i < argc; i++) {
        if (argv[i][4] == '=') {
            argv[i][4] = '\0';
            if (strcmp(argv[i], "-msl") == 0) {
                *msl = atoi(&argv[i][5]);
            }

            if (strcmp(argv[i], "-msf") == 0) {
                *msf = atoi(&argv[i][5]);
            }
        }
    }
}

/* ____________________________________________________________________________

    int main(int argc, char **argv)

    main entry point of application checks parameters and runs proper function
   ____________________________________________________________________________
*/
int main(int argc, char **argv) {
    FILE * file;
    char * filename;
    int msl = 3;
    int msf = 10;
    int result;

    if (argc <= 1) { /* Check if proper argument was passed otherwise print help and exit wth failure */
        printf("How to use:\n");
        printf("sistem.exe <corpus-file|[\"]word sequence[\"]> [-msf] [-msl]\n");
        return EXIT_FAILURE;
    }

    filename = malloc(sizeof(char) * (strlen(argv[1])+5));

    load_optional_arguments(argc, argv, &msl, &msf);

    /* test if input parameter is file */
    strcpy(filename, argv[1]);
    file = fopen(filename, "r");
    if (file == NULL) {
        strcat(filename, ".txt");
        file = fopen(filename, "r");
    }
    free(filename);

    if (file == NULL) { /* parameter is not file it must be word sequence to analyze */
        result = steaming_find_stems(msf, argv[1]);
        if (result == -1) {
            printf("Problem finding stems. Does stems.dat exists?\n");
            return EXIT_FAILURE;
        }
    } else { /* parameter is file for stem creation */
        steaming_create(file, msl);
    }

    return 0;
}