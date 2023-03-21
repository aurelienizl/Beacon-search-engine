#ifndef URL_EXTRACTOR_H
#define URL_EXTRACTOR_H

int check_domain(char **link, char *domain);
int add_link(char *file, int *i, struct list *links, char *domain, int len);
char *get_domain(char *link);
int check_tag(char *file, int *i, char *tag, size_t tag_len, int len);
struct list* bparser(char* file, char* link, int len);

#endif
