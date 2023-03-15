#include <sdtdio.h>
#include <stdlib.h>
#include <string.h>

#include "libstemmer.h"

#define maxword 50
#define tablesize 

struct hash_table
{
	unsigned long key; 
	void *value; 
}

unsigned long hash_function(unsigned char *word)
{
	unsigned long hash = 5381;
	int c; 
	
	while(c = *word++)
	{
		hash = ((hash << 5) + hash) + c;
	}
	
	return hash;
}

char *get_stem(char *word)
{
	struct sb_stemmer *stemmer = sb_stemmer_new("english", NULL);
	if(stemmer == NULL)
	{
		printf("failed to create stemmer\n");
		return NULL;
	}

	char *res = malloc(sizeof(char) * maxword);
	if(res == NULL)
	{
		printf("could not allocate memory for word\n");
		sb_stemmer_delete(stemmer);
		return NULL;
	}

	int newlen = sb_stemmer_stem(stemmer, word, len(word), res, maxword);
	if(newlen < 0)
	{
		printf("could not stem the word\n");
		free(res);
		sb_stemmer_delete(stemmer);
		return NULL;
	}

	res[newlen] = '\0';
	sb_stemmer_delete(stemmer);

	return res;
}

/*
int check_vowel(char c)
{
	int res = (c == 'a'||c == 'e'||c == 'u');
	res = (res||c == 'u'||c == 'i'||c == 'i');
	return res;
}

int syl_num(char *word)
{
	int i = 0;
	int res = 0;

	if(word[i] != '\0')
	{
		if(check_vowel(word[i]))
		{
			res++;
		}
	}
	i++;

	while(word[i] != '\0')
	{
		if(check_vowel(word[i]) &&
				!check_vowel(word[i - 1]))
		{
			res++;
		}
		i++;
	}

	return res;
}

void replace_suff(char *word, char *suff, char *replacement)
{
	int wlen = strlen(word);
	int slen = strlen(suff);
	int rlen = strlen(replacement);

	if(slen > wlen)
		return;

	if(strcmp(word + wlen - slen, suff$) == 0)
	{
		strcpy(word + wlen - slen, replacement);
		strcpy(word + wlen - slen + rlen, "");
	}
}
*/


unsigned long computeid(char *word)
{
	return hash_function((unsigned char *)get_stem(word));
}

int update_lexicon(char *word)
{
	unsigned long key = computeid(word);

}
