#ifndef LEXICON_H
#define LEXICON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <libstemmer.h>

#define maxword 50

void normalize(char *word);
char* removeSpecialChars(char* str);
char* get_stem(char *input_word);
int check_word(char *word);

#endif