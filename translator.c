#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "translator.h"

#define MAX 2056
#define true 1
#define false 0

char getCommandType(const char *codeLine) {
    char firstWord[MAX];
    sscanf(codeLine, "%s", firstWord);

    if (strcmp(firstWord, "add") == 0 ||
        strcmp(firstWord, "sub") == 0 ||
        strcmp(firstWord, "neg") == 0 ||
        strcmp(firstWord, "eq") == 0 ||
        strcmp(firstWord, "gt") == 0 ||
        strcmp(firstWord, "lt") == 0 ||
        strcmp(firstWord, "and") == 0 ||
        strcmp(firstWord, "or") == 0 ||
        strcmp(firstWord, "not") == 0 ) return 'L'; //Arithmetic and Logical Operations

    if (strcmp(firstWord, "label") == 0 ||
        strcmp(firstWord, "goto") == 0 ||
        strcmp(firstWord, "if-goto") == 0) return 'F'; //Flow of control

    if (strcmp(firstWord, "function") == 0 ||
        strcmp(firstWord, "call") == 0 ||
        strcmp(firstWord, "return") == 0) return 'C'; // Function Commands/Calls

    if (strcmp(firstWord, "pop") == 0 ||
        strcmp(firstWord, "push") == 0) return 'P'; //Push or Pop operation

    fprintf(stderr, "\033[31mUnrecognized command type:\033[0m %s\n", firstWord);
    return 'E'; //error
}

int LogicArithmeticToAsm (const char *codeLine, FILE *file) {
    char operation[MAX];
    char operator = 0;
    static int counter = 0; //unique code for label
    sscanf(codeLine, "%s", operation);

    if(strcmp(operation, "add") == 0 ) operator = '+';
    else if(strcmp(operation, "sub") == 0) operator = '-';
    else if(strcmp(operation, "and") == 0 ) operator = '&';
    else if(strcmp(operation, "or") == 0 ) operator = '|';
    else if(strcmp(operation, "not") == 0 ) operator = '!';
    else if(strcmp(operation, "eq") == 0 ) operator = '=';
    else if(strcmp(operation, "gt") == 0 ) operator = '>';
    else if(strcmp(operation, "lt") == 0 ) operator = '<';
    //neg case postponed

    if (operator == '+' || operator == '&' || operator == '|'){
        fprintf(file, "@SP\nAM=M-1\nD=M\nA=A-1\nM=D%cM\n", operator);
        return true;
    }

    if(operator == '-') {
        fprintf(file, "@SP\nAM=M-1\nD=M\nA=A-1\nM=M-D\n");
        return true;
    }

    if(strcmp(operation, "neg") == 0 ) operator = '-';

    if (operator=='-' || operator == '!') {
        fprintf(file, "@SP\nA=M-1\nM=%cM\n", operator);
        return true;
    }

    if(operator=='=' || operator=='>' || operator=='<') {
        fprintf(file, "@SP\nAM=M-1\nD=M\nA=A-1\nD=M-D\n@TRUE_COMPARISON_%d\n", counter);
        if(operator=='=') fprintf(file, "D;JEQ\n");
        if(operator=='>') fprintf(file, "D;JGT\n");
        if(operator=='<') fprintf(file, "D;JLT\n");
        fprintf(file, "@SP\nA=M-1\nM=0\n@END_COMPARISON_%d\n0;JMP\n(TRUE_COMPARISON_%d)\n@SP\nA=M-1\nM=-1\n(END_COMPARISON_%d)\n", counter, counter, counter);
        counter++;
        return true;
    }

    fprintf(stderr, "\033[31mUnrecognized operation:\033[0m %s\n", operation);
    return false;
}

int FlowToAsm(const char *codeLine, FILE *file, char functionName[]) {
    char operation[MAX];
    char destination[MAX];
    sscanf(codeLine, "%s %s", operation, destination);
    if(strcmp(functionName, "\0") != 0) {
        char newDest[MAX];
        strcpy(newDest, functionName);
        strcat(newDest, "$");
        strcat(newDest, destination);
        strcpy(destination, newDest);
    }
    if (strcmp(operation, "label") == 0) fprintf(file, "(%s)\n", destination);
    else if (strcmp(operation, "goto") == 0) fprintf(file, "@%s\n0;JMP\n", destination);
    else if (strcmp(operation, "if-goto") == 0) fprintf(file, "@SP\nAM=M-1\nD=M\n@%s\nD;JNE\n", destination);
    else {
        fprintf(stderr, "\033[31mUnrecognized operation:\033[0m %s\n", operation);
        return false;
    }
    return true;
}

int FunctionCallToAsm(const char *codeLine, FILE *file, char functionNameForLabel[]) {
    char operation[MAX];
    char functionName[MAX];
    char parameterNum[MAX];
    static int counter = 0;
    sscanf(codeLine, "%s %s %s", operation, functionName, parameterNum);

    if (strcmp(operation, "function") == 0) {
        fprintf(file, "(%s)\n@R13\nM=0\n"
                      "(%s_INITIALIZATION_LOOP)\n@%s\nD=A\n@R13\nD=D-M\n@%s_FUNCTION_START\nD;JEQ\n"
                      "@SP\nA=M\nM=0\n@SP\nM=M+1\n"
                      "@R13\nM=M+1\n"
                      "@%s_INITIALIZATION_LOOP\n0;JMP\n"
                      "(%s_FUNCTION_START)\n" , functionName, functionName, parameterNum, functionName, functionName, functionName);

        strcpy(functionNameForLabel, functionName);
    }
    else if (strcmp(operation, "call") == 0) {
        fprintf(file,
            "@FUNCTION_RETURN_%s_%d\nD=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n"
            "@LCL\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n"
            "@ARG\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n"
            "@THIS\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n"
            "@THAT\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n"

            "@%s\nD=A\n@SP\nD=M-D\n@5\nD=D-A\n@ARG\nM=D\n"
            "@SP\nD=M\n@LCL\nM=D\n"
            "@%s\n0;JMP\n"
            "(FUNCTION_RETURN_%s_%d)\n",
            functionName, counter, parameterNum, functionName, functionName, counter);
        counter++;
    }
    else if (strcmp(operation, "return") == 0) {
        fprintf(file,
            "@LCL\nD=M\n@R13\nM=D\n"
            "@5\nA=D-A\nD=M\n@R14\nM=D\n"
            "@SP\nAM=M-1\nD=M\n@ARG\nA=M\nM=D\n"
            "@ARG\nD=M\n@SP\nM=D+1\n"
            "@R13\nAM=M-1\nD=M\n@THAT\nM=D\n"
            "@R13\nAM=M-1\nD=M\n@THIS\nM=D\n"
            "@R13\nAM=M-1\nD=M\n@ARG\nM=D\n"
            "@R13\nAM=M-1\nD=M\n@LCL\nM=D\n"
            "@R14\nA=M\n0;JMP\n");
        strcpy(functionNameForLabel, "\0");
    }
    else {
        printf("\033[31mUnrecognized function operation: %s\033[0m\n", operation);
        return false;
    }

    return true;
}

int StackOperationToAsm(const char *codeLine, FILE *file, const char *fileName) {
    char operation[MAX];
    char segment[MAX];
    char address[MAX];
    sscanf(codeLine, "%s%s%s", operation, segment, address);
    char commonPart[] = "D=D+M\n@R13\nM=D\n@SP\nAM=M-1\nD=M\n@R13\nA=M\nM=D\n";

    if (strcmp(operation, "push") == 0) {
        if(strcmp(segment, "local")==0) fprintf(file, "@%s\nD=A\n@LCL\nA=D+M\nD=M\n", address);
        else if(strcmp(segment, "argument")==0) fprintf(file, "@%s\nD=A\n@ARG\nA=D+M\nD=M\n", address);
        else if(strcmp(segment, "this")==0) fprintf(file, "@%s\nD=A\n@THIS\nA=D+M\nD=M\n", address);
        else if(strcmp(segment, "that")==0) fprintf(file, "@%s\nD=A\n@THAT\nA=D+M\nD=M\n", address);
        else if(strcmp(segment, "static")==0) fprintf(file, "@%s.%s\nD=M\n", fileName, address);
        else if(strcmp(segment, "temp")==0) fprintf(file, "@%d\nD=M\n", atoi(address)+5);
        else if(strcmp(segment, "pointer")==0) {
            if(strcmp(address, "0") == 0) fprintf(file, "@THIS\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n");
            else if (strcmp(address, "1") == 0) fprintf(file, "@THAT\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n");
        }
        else if(strcmp(segment, "constant") == 0) fprintf(file, "@%s\nD=A\n", address);
        else {
            fprintf(stderr, "\033[31mUnknown operation of push:\033[0m %s\n", segment);
            return false;
        }
        fprintf(file, "@SP\nA=M\nM=D\n@SP\nM=M+1\n");
    }
    else if (strcmp(operation, "pop") == 0) {
        if(strcmp(segment, "local")==0) fprintf(file, "@%s\nD=A\n@LCL\n%s", address, commonPart);
        else if(strcmp(segment, "argument")==0) fprintf(file, "@%s\nD=A\n@ARG\n%s", address, commonPart);
        else if(strcmp(segment, "this")==0) fprintf(file, "@%s\nD=A\n@THIS\n%s", address, commonPart);
        else if(strcmp(segment, "that")==0) fprintf(file, "@%s\nD=A\n@THAT\n%s", address, commonPart);
        else if(strcmp(segment, "static")==0) fprintf(file, "@SP\nAM=M-1\nD=M\n@%s.%s\nM=D\n", fileName, address);
        else if(strcmp(segment, "temp")==0) fprintf(file, "@SP\nAM=M-1\nD=M\n@%d\nM=D\n", atoi(address)+5);
        else if(strcmp(segment, "pointer")==0) {
            if(strcmp(address, "0") == 0)   fprintf(file, "@SP\nAM=M-1\nD=M\n@THIS\nM=D\n");
            else if (strcmp(address, "1") == 0)  fprintf(file, "@SP\nAM=M-1\nD=M\n@THAT\nM=D\n");
        }
        else {
            fprintf(stderr, "\033[31mUnknown operation of pop:\033[0m %s\n", segment);
            return false;
        }
    }
    else {
        fprintf(stderr, "\033[31mUnknown operation nor push or pop:\033[0m %s\n", operation);
        return false;
    }
    return true;
}

int vmToAsm(const char *codeLine, FILE *fileOut, const char *fileName) {
    const char commandType = getCommandType(codeLine);
    static char functionName[MAX]="\0";
    switch (commandType) {
        case 'L':
            return LogicArithmeticToAsm(codeLine, fileOut);
        case 'F':
            return FlowToAsm(codeLine, fileOut, functionName);
        case 'C':
            return FunctionCallToAsm(codeLine, fileOut, functionName);
        case 'P':
            return StackOperationToAsm(codeLine, fileOut, fileName);
        case 'E':
            return false;
        default: return false; //Not necessary but added for formatting and security.
    }
};

void defaultStartFile(FILE *fileOut) {
    fprintf(fileOut, "@256\nD=A\n@SP\nM=D\n");
}

void defaultStartDir(FILE *fileOut){
    fprintf(fileOut, "@256\nD=A\n@SP\nM=D\n");
    fprintf(fileOut,"@FUNCTION_RETURN_SYS_INIT\nD=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n"
                           "@LCL\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n"
                           "@ARG\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n"
                           "@THIS\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n"
                           "@THAT\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n"

                           "@0\nD=A\n@SP\nD=M-D\n@5\nD=D-A\n@ARG\nM=D\n"
                           "@SP\nD=M\n@LCL\nM=D\n"
                           "@Sys.init\n0;JMP\n"
                           "(FUNCTION_RETURN_SYS_INIT)\n");
}