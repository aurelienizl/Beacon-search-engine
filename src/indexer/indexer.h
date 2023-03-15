#ifndef INDEXER_H
#define INDEXER_H

struct list 
{
	struct list *next;
	void *data;
}

struct list *newlist();
struct list *new_element(char *data);
void add_top(struct list *list, struct list *element);
int check_tag(char *file, int *i, char *tag, size_t len);
void get_tag(char *file, int *i, char c, char **tag);
void add_link(struct list *links, char *file, int *i);
void skip_tag(char *file, int *i, char *tag, struct list *links);
void add_words(struct list *links, char *file, int *i);
struct list *parser(char *file);


#endif
