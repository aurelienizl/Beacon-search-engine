#include <stdio.h>
#include <stdlib.h>
#include "word_extractor.h"

int main()
{
    FILE *file = fopen("stuff.html", "r");
    if (file == NULL) {
        return 1;
    }
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);
    char *fileContent = (char *) malloc(fileSize + 1);
    if (fileContent == NULL) {
        fclose(file);
        return 1;
    }
    fread(fileContent, sizeof(char), fileSize, file);
    fileContent[fileSize] = '\0';
    fclose(file);

    get_words("https://citizendium.org/wiki/Welcome_to_Citizendium", fileContent);

    return 0;
}