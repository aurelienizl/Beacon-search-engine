#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "link_extractor.h"

char htag[9] = "a href=\"";

char *build_link(char *link, char *domain)
{
	char *res = calloc(sizeof(char), strlen(link)+strlen(domain)+8);
	strcpy(res, "http://");
	strcat(res, domain);
	strcat(res, link);
	//printf("%s\n", res);
	return res;
}

int check_suffix(char *link)
{	
	int i = strlen(link) - 1;

	if(i > 3)
	{
		if (link[i] == 'l' && link[i - 1] == 'm' &&
				link[i - 2] == 't' &&
				link[i - 3] == 'h' &&
				link[i - 4] == '.' )
		{
			return 1;
		}
	}


	int countdot = 0;
	int j = 0;
	
	if(link[j] != '/' || link[j] == '.')
	{
		countdot = countdot - 2;
	}
	
	
	while(j < i)
	{
		if(link[j] == '.')
		{
			countdot++;
		}
		if(countdot == 1)
		{
		       return 0;
		}		
		j++;
	}

	return 1;
}

int check_domain(char **link, char *domain)
{
	if(strlen(*link) < 3 || strstr((*link), "\n"))
		return 0;
	
	if((*link)[0] == '/' || ((*link)[0] == '.' && (*link)[1] == '/'))
	{ 
		if(check_suffix(*link))
			return 2;
		return 0;
	}

	char start[5] = "http";
	size_t i = 0;
	
	while(i < 4)
	{
		if((*link)[i] < 97)
		{
			if(start[i] != ((*link)[i] + 32))
			{
				break;
			}
		}
		else
		{
			if(start[i] != (*link)[i])
			{
				break;
			}
		}	
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
		return check_suffix(*link);
	}

	return 0;
}

int add_link(char *file, int *i, struct stack** links, char *domain, int len)
{
	int pos = 0;
	char *link = calloc(sizeof(char), 256);
	int state;

	while(*i < len && pos < 256)
	{
		if(file[*i] < 33 || file[*i] > 126)
		{
			free(link);
			return 0;
		}

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
		//printf("found link: %s\n", link);
		state = check_domain(&link, domain);
		if(state)
		{
			if(state == 2)
			{
				
				addstack((*links), build_link(link, domain));
				free(link);
			}
			else
			{
				printf("%s\n", link);
				addstack((*links), link);
			}
			return 1;
		}
		else
		{
			free(link);
			return 0;
		}
	}

	free(link);
	return 0;
}

char *get_domain(char *link)
{
	char start[5] = "http";
	char *domain = calloc(sizeof(char), strlen(link) + 1);
	size_t i = 0;

	while(i < 4)
	{
		if(link[i] < 97)
		{
			if(start[i] != (link[i] + 32))
			{
				break;
			}
		}
		else
		{
			if(start[i] != link[i])
			{
				break;
			}
		}	
		i++;
	}

	if(i != 4)
		return NULL;

	if(link[i] == 's')
		i++;



	size_t j = 0;
	i += 3;
	while(link[i] != '\0')
	{
		if(link[i] == '/')
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

void bparser(struct stack** url_list, char* file, char* link, int len)
{
	char *domain = get_domain(link);

	if(domain == NULL)
	{
		printf("could not retrieve domain name, the link list returned is empty\n");
		return;
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
				return;
			}

			switch(file[*i])
			{
				case 'a':
					if(check_tag(file, i, htag, 8, len))
					{
						add_link(file, i, url_list, domain, len);
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

	return;
}
