#ifndef _STORE_SERVER_H_
#define _STORE_SERVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>
#include <openssl/sha.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>
#include <errno.h>
#include <ctype.h>


#include "../structures/string/string.h"




/*
** @note: The following section contains declarations of crypto.c file
*/

char* sha1_hash(const unsigned char *data, size_t len);

char *compress_array(char *data, size_t data_len);

char* uncompress_array(char* data, size_t data_len);

char* base64_encode(char* input);

char* base64_decode(char* input);

/*
** @note: The following section contains declarations of store_server.c file
*/

void create_directory(char* url);

int exist_webpage(char *url);

int write_to_file(char* data, char* url);


#endif