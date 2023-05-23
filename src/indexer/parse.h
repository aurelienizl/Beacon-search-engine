#ifndef PARSE_H
#define PARSE_H

#include <stdio.h>
#include <stdlib.h>
#include "word_extractor.h"
#include "table.h"

char* base64_decode(char* input);
void read_file(const char *filename, char **content);
void parse(const char* path);

#endif