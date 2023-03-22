#ifndef URL_EXTRACTOR_H
#define URL_EXTRACTOR_H

#include "../structures/stack/stack.h"

int check_domain(char **link, char *domain);
int add_link(char *file, int *i, struct stack** links, char *domain, int len);
char *get_domain(char *link);
int check_tag(char *file, int *i, char *tag, size_t tag_len, int len);
void bparser(struct stack** url_list, char* file, char* link, int len);

#endif
