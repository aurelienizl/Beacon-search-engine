#include <libxml/HTMLparser.h>
#include <libxml/xpath.h>
#include <libxml/uri.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <signal.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>
#include <openssl/sha.h>
#include <dirent.h>
#include <pthread.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>


// tools fonctions 

char* base64_decode(char* input);

char* base64_encode(char* input);

char* sha1_hash(const unsigned char *data, size_t len);

char *string_to_heap(char *string);

char *string_concat(char *str1, char *str2);

// crawler fonctions

typedef struct
{
	char *buf;
	size_t size;
} memory;

void sighandler(int dummy);

size_t grow_buffer(void *contents, size_t sz, size_t nmemb, void *ctx);

CURL *make_handle(char *url);

void log_message(const char* message);

int is_php_url(const char* url);

int is_already_crawled(const char* url);

size_t follow_links(CURLM* multi_handle, memory* mem, char* url, char* domain);

int is_html(char *ctype);

void* crawl(void *url);

int is_domain(const char *url, const char *domain);

char *get_domain(const char *link);

void rek_mkdir(char *path);

void create_directory(char *url);

void write_to_path(char* path, char* data);

int write_html_to_file(char* data, char* url);

int exist_webpage(char *url);