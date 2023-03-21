#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../structures/lists/linked-list/list.h"

char htag[9] = "a href=\"";

char *build_link(char *link, char *domain)
{
	char *res = malloc(strlen(link)+strlen(domain)+5);
	strcpy(res, "http");
	strcat(res, domain);
	strcat(res, link);
	printf("new link = %s\n", res);
	return res;
}

int check_domain(char **link, char *domain)
{
	if((*link)[0] == '/')
	{ 
		
		return 2;
	}

	char start[5] = "http";
	size_t i = 0;
	
	while((*link)[i] == start[i])
	{
		i++;
	}

	if(i != 4)
	{
		return 0;
	}

	if((*link)[i] == 's')
	{
		i++;
	}

	size_t j = 0;
	while(domain[j] == (*link)[i] && (*link)[i] != '\0')
	{
		i++;
		j++;
	}

	if((*link)[i] == '\0' || domain[j] == '\0')
	{
		return 1;
	}

	return 0;
}

int add_link(char *file, int *i, struct list *links, char *domain, int len)
{
	int pos = 0;
	char *link = malloc(sizeof(char)*256);
	int state;

	while(*i < len && pos < 256)
	{
		if(file[*i] == '\"')
		{
			(*i)++;
			if(file[*i] == '>' || file[*i] == ' ')
			{
				link[pos] = '\0';
				break;
			}
			link[pos] = '\"';
			pos++;
		}
		link[pos] = file[*i];
		pos++;
		(*i)++;
	}

	if(pos < 256)
	{
		link = realloc(link, pos);
		printf("found link: %s\n", link);
		state = check_domain(&link, domain);
		if(state)
		{
			if(state == 2)
			{
				
				add_top(links, new_element(build_link(link, domain)));
				free(link);
			}
			else
			{
				add_top(links, new_element(link));
			}
			return 1;
		}
		else
		{
			return 0;
		}
	}

	return 0;
}

char *get_domain(char *link)
{
	char start[5] = "http";
	char *domain = malloc(sizeof(char) * strlen(link) + 1);
	size_t i = 0;
	int nslash = 0;

	while(link[i] == start[i])
	{
		i++;
	}

	if(i != 4)
		return NULL;

	if(link[i] == 's')
		i++;

	size_t j = 0;
	while(link[i] != '\0')
	{
		if(link[i] == '/')
		{
			nslash++;
		}
		if(nslash == 3)
		{
			break;
		}
		domain[j] = link[i];
		i++;
		j++;
	}

	domain[j] = '\0';
	domain = realloc(domain, sizeof(char) * (j+1));
	return domain;
}

int check_tag(char *file, int *i, char *tag, size_t tag_len, int len)
{
	size_t j = 1; 
	(*i)++;

	while(*i < len && j < tag_len)
	{
		if(file[*i] != tag[j])
			break;
		j++;
		(*i)++;

	}

	return (j == tag_len);
}

struct list* bparser(char* file, char* link, int len)
{
	char *domain = get_domain(link);

	struct list *links = new_list();

	if(domain == NULL)
	{
		printf("could not retrieve domain name, the link list returned is empty\n");
		return links;
	}

	int *i = calloc(sizeof(int), 1);
	(*i) = 0;

	while(*i < len)
	{
		if(file[*i] == '<')
		{
			(*i)++;

			if(*i >= len)
			{
				return links;
			}

			switch(file[*i])
			{
				case 'a':
					if(check_tag(file, i, htag, 8, len))
					{
						add_link(file, i, links, domain, len);
					}
					break;
			}

		}
		
		(*i)++;
	}

	free(i);
	free(domain);

	
	/*
	printf("\n\n\nPRINTING THE RESULT\n");
	struct list* current = links->next;
	while(current != NULL)
	{
		printf("%s\n", (char *)current->data);
		current = current->next;
	}
	free(current);
	*/

	return links->next;
}
