#ifndef INDEXER_H
#define INDEXER_H

char* get_domain(char *link);
int check_domain(char* link, char *domain);
int check_tag(char *file, int *i, char *tag, size_t len);
void get_tag(char *file, int *i, char c, char **tag);
void add_link(struct list *links, char *file, int *i, char *domain);
void skip_tag(char *file, int *i, char *tag, struct list *links, char *domain);
void add_words(struct list *links, char *file, int *i, char *domain);
struct list *parser(char *file, char* link);


#endif
