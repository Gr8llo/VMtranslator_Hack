#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "setup.h"

#define true 1
#define false 0

int checkExtension(const char *inputFile) {
    const char *dot = strrchr(inputFile, '.');
    if(dot == NULL) {
        fprintf(stderr, "Invalid extension\n");
        return false;
    }
    if(strcmp(dot, ".vm") != 0) {
        fprintf(stderr, "Invalid extension\n");
        return false;
    }
    return true;
}

char *setup (const char *inputFile){
    if(!checkExtension(inputFile)) {
           return NULL;
    }//check extension

    char fileName[strlen(inputFile)+1];
    strcpy(fileName, inputFile);
    char *dot = strrchr(fileName, '.');
    if(dot != NULL) {
        *dot = '\0';
    }

    char *fileExtension = ".asm";
    char *fileOutSrc = (char *)malloc(strlen(fileName) + strlen(fileExtension) + 1);
    if (fileOutSrc == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    } //check memory allocation
    //create file name with extension
    strcat(strcpy(fileOutSrc, fileName), fileExtension);
    return fileOutSrc;
}
