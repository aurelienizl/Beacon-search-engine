/*
 * Author: Izoulet Aurélien
 * Purpose: Store server.
 * Language: C.
 */

#ifndef _STORE_SERVER_H_
#define _STORE_SERVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>
#include <openssl/sha.h>
#include <sys/stat.h>
#include <dirent.h>
#include "../structures/string/string.h"

/**
** @brief          Store server page structure.
** @param url      Full url from the webpage.
** @param content  Full html text from the webpage.
** @param checksum Checksum of the webpage.
*/

struct webpage
{
    char* url;
    size_t url_size;

    struct data *content;

    char* checksum;
    size_t checksum_size;
};

/**
** @brief                Compressed data structure.
** @param data           Represent the data of the page.
** @param data_size      Size of the data at the actual context (compressed or not)
** @param data_capacity  Maximum capacity of the data. (Size of the decompressed data)
*/

struct data
{
    unsigned char* data;
    size_t data_size;
    size_t data_capacity;
};

/*
** @note: The following section contains declarations of compression.c file
*/

void compress_data(struct data *data);

void decompress_data(struct data *data);

/*
** @note: The following section contains declarations of crypto.c file
*/

char* sha1_hash(const unsigned char *data, size_t len);

/*
** @note: The following section contains declarations of store_server.c file
*/

struct webpage create_webpage(char* url, size_t url_size, char* content, size_t content_size);

unsigned char* read_webpage(char* url, size_t url_size);

void normalize_html(char* html, size_t *html_size);

void write_webpage(struct webpage *webpage);

int exist_webpage(char* url, size_t url_size);

/*
** @note: The following section contains declarations of io.c file
*/

char *io_get_file(char *path);


#endif