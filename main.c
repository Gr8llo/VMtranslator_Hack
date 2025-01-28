#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "setup.h"
#include "translator.h"

#define MAX 2056

int main(const int argc, char *argv[])
 {
    //check number of arguments
    if(argc < 2) {
        fprintf(stderr, "Invalid number of arguments");
        return 1; //generic error
    }
    char *fileOutName = setup(argv[1]);

    FILE *fileIn = fopen(argv[1], "r");
    if (fileIn == NULL) {
        fprintf(stderr, "File %s could not be opened\n", argv[1]);
        free(fileOutName);
        return 1;
    } //check memory allocation

    FILE *fileOut = fopen(fileOutName, "w");
    if (fileOut == NULL) {
        fprintf(stderr, "File %s could not be created\n", fileOutName);
        free(fileOutName);
        return 1;
    } //check memory allocation

    char *extension = strstr(fileOutName, ".asm");
    *extension = '\0';
    char line[MAX];
    int lineCount = 1;

    defaultStart(fileOut);
    while (fgets(line, MAX, fileIn) != NULL) {
        {
            char *commentStart = strstr(line, "//");
            if(commentStart != NULL) *commentStart = '\0';
            if (line[0] == '\0' || line[0] == '\n' || line[0] == '\r' || line[0] == '(') continue; //continue skip to the next cicle
        }//handle comments
        if(!vmToAsm(line, fileOut, fileOutName)) {
            fprintf(stderr, "\033[31mError at line \033[1m%d\033[0m\n", lineCount);
            return 1;
        }
        lineCount++;
    }

    defaultEnd(fileOut);
    fclose(fileIn);
    fclose(fileOut);
    printf("\033[32mTranslation to asm ended\033[0m\n");
    return 0;
}

