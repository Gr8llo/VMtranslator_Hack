#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
//sono stato obbligato ad aggiungere questa libreria per gestire le directory

#include "setup.h"
#include "translator.h"

#define MAX 2056
#define true 1
#define false 0

int translate(char *inputFile, char *outputFile, const char *fileName) {

    FILE *fileIn = fopen(inputFile, "r");
    if (fileIn == NULL) {
        fprintf(stderr, "File %s could not be opened\n", inputFile);
        return false;
    } //check memory allocation

    FILE *fileOut = fopen(outputFile, "a");
    if (fileOut == NULL) {
        fprintf(stderr, "File %s could not be opened\n", outputFile);
        fclose(fileIn);
        return false;
    } //check memory allocation

    char line[MAX];
    int lineCount = 1;
    while (fgets(line, MAX, fileIn) != NULL) {
        {
            char *commentStart = strstr(line, "//");
            if(commentStart != NULL) *commentStart = '\0';
            if (line[0] == '\0' || line[0] == '\n' || line[0] == '\r' || line[0] == '(') continue; //continue skip to the next cycle
        }//handle comments
        if(!vmToAsm(line, fileOut, fileName)) {
            fprintf(stderr, "\033[31mError at line \033[1m%d\033[0m\n", lineCount);
            return false;
        }
        lineCount++;
    }

    fclose(fileIn);
    fclose(fileOut);
    return true;
}

int translateFile(char *fileName)
{
    char *fileOutName = setup(fileName);
    if (fileOutName == NULL) {
        return false;
    }

    FILE *outPutFile= fopen(fileOutName, "w");
    if (outPutFile == NULL) {
        fprintf(stderr, "File %s could not be created\n", fileOutName);
        return false;
    } //check memory allocation
    defaultStartFile(outPutFile);
    fclose(outPutFile);

    char Name[MAX];
    strcpy(Name, fileName);
    char *extension = strstr(fileName, ".vm");
    if (extension != NULL) *extension = '\0';

    return translate(fileName, fileOutName, Name);
}

int translateDir(const char *dirName)
{
    char *slash = strchr(dirName, '/');
    if(slash != NULL) *slash = '\0';
    char dirOutName[MAX];
    strcpy(dirOutName, dirName);
    strcat(dirOutName, ".asm");
    DIR *dir = opendir(dirName);
    if (dir == NULL) {
        printf("Could not open directory %s\n", dirName);
        return false;
    }
    FILE *outPutFile = fopen(dirOutName, "w");
    if (outPutFile == NULL) {
        fprintf(stderr, "File %s could not be created\n", dirOutName);
        return false;
    } //check memory allocation
    defaultStartDir(outPutFile);
    fclose(outPutFile);

    struct dirent *entry;
    while((entry = readdir(dir)) != NULL){
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue; //skip hidden directory
        if(checkExtension(entry->d_name)) { //skip all non .vm files
            char fullPath[MAX];
            strcpy(fullPath, dirName);
            strcat(fullPath, "/");
            strcat(fullPath, entry->d_name);
            translate(fullPath, dirOutName, entry->d_name);
        }
    }
    closedir(dir);
    return true;
}


int main(const int argc, char *argv[])
 {
    //check number of arguments
    if(argc < 2) {
        fprintf(stderr, "Invalid number of arguments");
        return 1; //generic error
    }

    DIR *testDir = opendir(argv[1]);
    //try to open as a dir if it fails it is a file
    if (testDir != NULL) {
        closedir(testDir);
        printf("\033[0;32mDir successfully translated\033[0m\n");
        return translateDir(argv[1]);
    }
    FILE *testFile = fopen(argv[1], "r");
    if (testFile != NULL) {
        fclose(testFile);
        printf("\033[0;32mFile successfully translated\033[0m\n");
        return translateFile(argv[1]);
    }
    printf("File or Dir %s could not be opened\n", argv[1]);
    return 1;
}