#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bool.h"
#include "steming.h"

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

int main(int argc, char **argv) {
    FILE * file;
    char * filename;
    int msl = 3;
    int msf = 10;

    if (argc <= 2) {
        printf("How to use:\n");
        printf("sistem.exe <corpus-file|[\"]word sequence[\"]> [-msf] [-msl]\n");
        return EXIT_FAILURE;
    }

    filename = malloc(sizeof(char) * (strlen(argv[1])+5));

    load_optional_arguments(argc, argv, &msl, &msf);

    strcpy(filename, argv[1]);
    file = fopen(filename, "r");
    if (file == NULL) {
        strcat(filename, ".txt");
        file = fopen(filename, "r");
    }
    free(filename);

    if (file == NULL) {
        find_stems(msf, argv[1]);
    } else {
        create_stems(file, msl);
    }

    return 0;
}