/*
 * Author: Izoulet Aurélien
 * Purpose: Strings implementation
 * Language: C.
 */

#ifndef _STRING_H_
#define _STRING_H_

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>

char* string_concat(char* str1, char* str2);

char *string_to_heap(char *string, size_t string_size);

#endif
