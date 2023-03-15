#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char htag[9] = "a href=\"";

struct list *new_list()
{
	struct list *head = NULL; 
	head = malloc(sizeof(struct list));
	head->next = NULL;
	head->data = NULL;
	return head;
}

struct list *new_element(char *data)
{
	struct list *head = NULL;
	head = malloc(sizeof(struct list));
	head->next = NULL;
	head->data = data;
	return head;
}

void add_top(struct list *list, struct list *element)
{
	if(list->next == NULL)
	{
		list->next = element;
		return;
	}
	element->next = list->next;
	list->next = element;
}

//checks after the offset if tag is a prefix
//length of the tag is given
int check_tag(char *file, int *i, char *tag, size_t len)
{
	int j = 1;
	char c = file[*i];

	while(c != '\0' && j < len)
	{
		c = file[*i];
		if(c != tag[j])
			break;
		j++;
		(*i)++;
	}
	return (j == len);
}

//retrieves simple tag keywords and stores them in tag
//c represents the first keyword character already encountered after <
void get_tag(char* file, int*i, char c, char **tag)
{
	(*tag)[0] = c;
	(*i)++;
	c = file[*i];
	int pos = 1;

	while(c != '\0' && c != ' ')
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

	while(c != '\0' && c != '>')
	{
		(*i)++;
		c = file[*i];
	}
}

//add the link conrained between the a tags to links
void add_link(struct list *links, char *file, int *i)
{
	char c;
	int pos = 0;
	char *link = malloc(sizeof(char)*256);
	
	(*i)++;
	while((c = file[*i]) != '\0' && pos < 256)
	{
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
		add_top(links, new_element(link));
	}

	//moving out of the tag : add attribute processing later
	if(c == ' ')
	{
		while(c != '>')
		{
			(*i)++;
			c = file[*i];
			//ATTRIBUTE PROCESSING GOES HERE
		}
	}
	(*i)++;
	c = file[*i];

	//move the offset to the end of the closing tag
	while(c != '>')
	{
		//printf("%c", c);
		(*i)++;
		c = file[*i];
	}
	//printf("\n");
}

//skips uninteresting tags, stops at >
void skip_tag(char *file, int *i, char *tag, struct list *links)
{
	printf("\n|skipping, tag = %s| --------------------\n", tag);
	char c;

	//skip or process tag content
	(*i)++;
	while((c = file[*i]) != '\0')
	{
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
					add_link(links, file, i);
					(*i)++;
					if((c = file[*i]) == '\0')
						return;
					//to solve for successive tags
					if(c == '<')
						goto check_again;
				}

			}
		}
		printf("%c",c);
		//CONTENT PROCESSING GOES HERE
		(*i)++;

	}

	//skip the last tag character
	(*i)++;
	c = file[*i];
	printf("\n---------------------------------------\n");
}

//parses found paragraph and extracts links
void add_words(struct list *links, char *file, int *i)
{
	(*i)++;
	char c = file[*i];
	printf("\nparagraph========================\n");

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
	while((c = file[*i]) != '\0' && pos < 64)
	{
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
						add_link(links, file, i);
						(*i)++;
						if((c = file[*i]) == '\0')
							return;
						//to solve for successive tags
						if(c == '<')
							goto tag_processing;
						break;
					}
					else
					{
						skip_tag(file, i, "a", links);
					}
				//end of paragraph
				case '/':
					//skip over paragraph closing tag
					(*i) = (*i) + 2;
					c = file[*i];
					printf("\n==================================\n");
					return;	
				//skip uninteresting tag
				default:
					get_tag(file, i, c, &tag);
					skip_tag(file, i, tag, links);
					(*i)++;
					if((c = file[*i]) == '\0')
						return;
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

	}

	//free
	if(word != NULL)
		free(word);

}

//parses the whole file
struct list *parser(char* file)
{
	char c;

	struct list *links = new_list();

	int *i = calloc(sizeof(int), 1);
	(*i) = 0;

	while((c = file[*i]) != '\0')
	{
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
						add_link(links, file, i);
					}
					break;
				//extract paragraph
				case 'p':
					add_words(links, file, i);
					(*i)++;
					if((c = file[*i]) == '\0')
						return links;
					break;
			}
		}
		(*i)++;
	}
	free(i);
	return links;
}
