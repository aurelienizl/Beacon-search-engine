#include <libxml/HTMLparser.h>
#include <libxml/xpath.h>
#include <libxml/uri.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <signal.h>
#include <ctype.h>

#include "../database/store_server.h"

int crawl(char *url);

char *get_domain(const char *link);

int is_domain(const char *url,const char *domain);

char *clean_php_url_for_crawler(char *url); 