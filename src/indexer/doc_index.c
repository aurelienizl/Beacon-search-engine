#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//hit structure
struct hit
{
	int pos;
	struct hit *next;
}

//linked list for the forward index
struct docindex
{
	struct docindex *next;
	int wordid;
	int info;
	int nhits
	struct hit *hits;
}

struct hit *new_hits()
{
	struct hit* head = NULL;
	head = malloc(sizeof(struct hit));
	
	head->pos = -1;
	head->next = NULL;
	
	return head;

}

void add_hit(struct hit *hits, int pos)
{
	newhit = new_hits();
	newhit->pos = pos;
	
	if(hits->next == NULL)
	{
		hits->next = newhit;
		return;
	}

	newhit->next = hits->next;
	hits->next = newhit;

	return head;
}

struct docindex *new_doc()
{
	struct docindex *head = NULL;
	head = malloc(sizeof(struct docindex));
	
	head->next = NULL;
	head->wordid = NULL;
	head->info = NULL;
	head->nhits = 0;
	head->hits = new_hits();
	
	return head;
}

struct docindex *new_word(char *word, int info, int pos)
{
	struct docindex *head = NULL;
	head = malloc(sizeof(struct docindex));

	head->next = NULL;
	head->wordid = computeid(word);
	head->info = info;
	head->nhits = 1;
	head->hits = new_hits();
	add_hit(head->hits, pos);

	return head;
}

void add_word(struct docindex *docindex, struct docindex* word)
{
	if(docindex->next == NULL)
	{
		docindex->next = word;
		return;
	}
	word->next = docindex->next;
	docindex->next = word;
}
