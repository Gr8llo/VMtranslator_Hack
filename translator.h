#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#define true 1
#define false 0

int vmToAsm(const char *codeLine, FILE *fileOut, const char *fileName);

void defaultStartFile(FILE *fileOut);
void defaultStartDir(FILE *fileOut);

#endif //TRANSLATOR_H