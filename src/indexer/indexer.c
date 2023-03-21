#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../structures/lists/linked-list/list.h"

char htag[9] = "a href=\"";

char* get_domain(char *link)
{
	char start[5] = "http";
	char *domain = malloc(sizeof(char) * strlen(link) + 1);
	size_t i = 0;
	int nslash = 0;

	while(link[i] == start[i])
	{	
		i++;
	}

	if(link[i] == 's')
		i++;
	
	if(i != 4)
		return NULL;

	size_t j = 0;
	while(link[i] != '\0' && nslash < 3)
	{
		if(link[i] == '/')
			nslash++;

		domain[j] = link[i];
		i++;
		j++;
	}

	domain[j] = '\0';

	domain = realloc(domain, sizeof(char) *(j+1));
	printf("link :%s\n", link);
	printf("domain :%s\n", domain);
	return domain;
}

int check_domain(char *link, char *domain)
{
	printf("checking domain\n");
	size_t i = 0;
	char start[5] = "http"; 	
	while(link[i] == start[i])
	{
		i++;
	}
	printf("http verified\n");

	if(link[i] == 's')
	{
		i++;
		printf("skipping s\n");
	}

	size_t j = 0;
	while(domain[j] == link[i] && link[i] != '\0')
	{
		i++;
		j++;
		printf("%c",domain[i]);
	}
	printf("left loop 1\n");

	if(link[i] == '\0' || domain[j] == '\0')
	{
		return 1;
	}


	return 0;
}

//checks after the offset if tag is a prefix
//length of the tag is given
int check_tag(char *file, int *i, char *tag, size_t len)
{
	printf("= checking tag %s :", tag);
	size_t j = 1;
	(*i)++;
	char c = file[*i];
	int l = strlen(file);

	while(*i < l && j < len)
	{
		c = file[*i];
		if(c != tag[j])
			break;
		j++;
		(*i)++;
	}

	printf(" %i\n", (j == len));
	return (j == len);
}

//retrieves simple tag keywords and stores them in tag
//c represents the first keyword character already encountered after <
void get_tag(char* file, int*i, char c, char **tag)
{
	(*tag)[0] = c;
	(*i)++;
	c = file[*i];
	int l = strlen(file);
	int pos = 1;

	while(*i < l  && c != ' ')
	{
		if(c == '>')
			break;
		(*tag)[pos] = c;
		pos++;
		(*i)++;
		c = file[*i];
	}

	(*tag)[pos] = '\0';
	(*tag) = realloc((*tag), pos);

	/*
	while(*i < l && c != '>')
	{
		(*i)++;
		c = file[*i];
	}
	*/
}

//add the link conrained between the a tags to links
void add_link(struct list *links, char *file, int *i, char *domain)
{
	printf("==== potential link addition: \n");
	char c;
	int pos = 0;
	int l = strlen(file);
	char *link = malloc(sizeof(char)*256);
	
	//(*i)++;
	while(*i < l && pos < 256)
	{
		c = file[*i];
		if(c == '\"')
		{
			(*i)++;
			c = file[*i];
			//it is the end of the link or the a tag
			if (c == '>' || c == ' ')
			{
				link[pos] = '\0';
				break;
			}
			//it is part of the link
			else
			{
				link[pos] = '\"';
			}
		}
		link[pos] = c;
		pos++;
		(*i)++;
	}
	if(pos != 256)
	{
		//in case there's still space in link
		link = realloc(link, pos);
		printf("resulting link : %s , added?\n", link);
		if(check_domain(link, domain))
		{
			add_top(links, new_element(link));
			printf("yes\n");
		}
		else
		{
			printf("no\n");
		}
	}

	printf("attributes : \n");
	//moving out of the tag : add attribute processing later
	if(c == ' ')
	{
		while(c != '>')
		{
			(*i)++;
			c = file[*i];
			printf("%c",c);
			//ATTRIBUTE PROCESSING GOES HERE
		}
	}
	(*i)++;
	c = file[*i];

	printf("\nremainder : \n");
	//move the offset to the end of the closing tag
	while(c != '>')
	{
		(*i)++;
		c = file[*i];
		printf("%c",c);
	}
	printf("\n\n");
}

//skips uninteresting tags, stops at >
void skip_tag(char *file, int *i, char *tag, struct list *links, char *domain)
{
	printf("=== skipping tag %s :\n", tag);
	char c;
	int l = strlen(file);

	//skip or process tag content
	(*i)++;
	while(*i < l)
	{
		c = file[*i];
check_again:
		if(c == '<')
		{
			//check for closing tag
			(*i)++;
			if ((c = file[*i]) == '/')
			{
				(*i)++;
				if((c = file[*i]) == tag[0])
				{
					if(check_tag(file, i, tag, strlen(tag)))
						break;
				}
			}
			//check for link
			if(c == 'a')
			{
				if(check_tag(file, i, htag, 8))
				{
					add_link(links, file, i, domain);
					(*i)++;
					if(*i >= l)
						return;
					c = file[*i];
					//to solve for successive tags
					if(c == '<')
						goto check_again;
				}

			}
		}
		//CONTENT PROCESSING GOES HERE
		(*i)++;

	}

	//skip the last tag character
	c = file[*i];
	printf("\n\n");
}

//parses found paragraph and extracts links
void add_words(struct list *links, char *file, int *i, char *domain)
{
	printf("=== extracting words starting from %c :\n", file[*i + 1]);
	(*i)++;
	char c = file[*i];
	int l = strlen(file);

	//skip over paragraph opening tag
	while(c != '>')
	{
		(*i)++;
		c = file[*i];
	}

	int pos = 0;
	char *word = malloc(sizeof(char)*64);
	char *tag = malloc(sizeof(char)*16);
	
	(*i)++;
	while(*i < l && pos < 64)
	{
		c = file[*i];
		//treat tags
		if(c == '<')
		{
			if(pos != 0)
			{
				word[pos] = '\0';
				printf("%s ", word);
				pos = 0;
			}
tag_processing:
			(*i)++;
			switch(c = file[*i])
			{
				//extract link from paragraph
				//LINK KEYWORD EXTACTION GOES HERE
				case 'a':
					if(check_tag(file, i, htag, 8))
					{
						add_link(links, file, i, domain);
						(*i)++;
						if(*i >= l)
						{
							return;
						}
						c = file[*i];
						//to solve for successive tags
						if(c == '<')
							goto tag_processing;
						break;
					}
					else
					{
						skip_tag(file, i, "a", links, domain);
						break;
					}
				//end of paragraph
				case '/':
					//skip over paragraph closing tag
					(*i) = (*i) + 2;
					c = file[*i];
					printf("==== word extraction ended");
					return;	
				//skip uninteresting tag
				default:
					get_tag(file, i, c, &tag);
					skip_tag(file, i, tag, links, domain);
					(*i)++;
					if(*i >= l)
						return;
					c = file[*i];
					//to solve for successive tags
					if(c == '<')
						goto tag_processing;

				(*i)++;
			}
		}

		//save word
		if(c == ' ')
		{
			if(pos != 0)
			{
				word[pos] = '\0';
				printf("%s ", word);
				pos = 0;
			}
		}
		//construct word
		else
		{
			word[pos] = c;
			pos++;
		}

		(*i)++;
	}

	//free
	if(word != NULL)
		free(word);
	printf("\n\n");

}

//parses the whole file
struct list *parser(char* file, char* link)
{
	char c;
	char *domain = get_domain(link);
	printf("first letter of domain is %c", domain[0]);
	int l = strlen(file);

	struct list *links = new_list();

	int *i = calloc(sizeof(int), 1);
	(*i) = 0;

	while(*i < l)
	{
		c = file[*i];
		if (c == '<')
		{
			(*i)++;
			c = file[*i];
			switch(c)
			{
				//extract link
				case 'a':
					if(check_tag(file, i, htag, 8))
					{
						add_link(links, file, i, domain);
					}
					break;
				
				//extract paragraph
				case 'p':
					add_words(links, file, i, domain);
					(*i)++;
					if(*i >= l)
						return links;
					break;
			}
		}
		(*i)++;
	}
	free(i);
	free(domain);
	
	return links;
}
