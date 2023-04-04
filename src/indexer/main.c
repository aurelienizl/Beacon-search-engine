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

    
    FILE *file2 = fopen("stuff2.html", "r");
    if (file2 == NULL) {
        return 1;
    }
    fseek(file2, 0, SEEK_END);
    long fileSize2 = ftell(file2);
    rewind(file2);
    char *fileContent2 = (char *) malloc(fileSize2 + 1);
    if (fileContent2 == NULL) {
        fclose(file2);
        return 1;
    }
    fread(fileContent2, sizeof(char), fileSize2, file2);
    fileContent2[fileSize2] = '\0';
    fclose(file2);
    

    
    FILE *file3 = fopen("stuff3.html", "r");
    if (file3 == NULL) {
        return 1;
    }
    fseek(file3, 0, SEEK_END);
    long fileSize3 = ftell(file3);
    rewind(file3);
    char *fileContent3 = (char *) malloc(fileSize3 + 1);
    if (fileContent3 == NULL) {
        fclose(file3);
        return 1;
    }
    fread(fileContent3, sizeof(char), fileSize3, file3);
    fileContent3[fileSize3] = '\0';
    fclose(file3);
    

    get_words("http://www.scholarpedia.org/article/Main_Page", fileContent);
    printf("\n");
    get_words("https://citizendium.org/wiki/Union_Faith", fileContent2);
    printf("\n");
    get_words("https://citizendium.org/wiki/Welcome_to_Citizendium", fileContent3);
    create_word_db("../barrels");

    return 0;
}