#ifndef LEXICON_H
#define LEXICON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libstemmer.h>

#define maxword 50

char* get_stem(char *input_word);

#endif