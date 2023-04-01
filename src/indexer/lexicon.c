#include "lexicon.h"

char *get_stem(char *word) {
    struct sb_stemmer *stemmer = sb_stemmer_new("english", NULL);
    if (!stemmer) {
        fprintf(stderr, "Error creating stemmer.\n");
        return NULL;
    }
    char *stemmed_word = (char *) malloc(sizeof(char) * (strlen(word) + 1));
    if (!stemmed_word) {
        fprintf(stderr, "Error allocating memory for stemmed word.\n");
        sb_stemmer_delete(stemmer);
        return NULL;
    }
    const sb_symbol* stemmed = sb_stemmer_stem(stemmer, (unsigned char *) word, strlen(word));
    if (stemmed_word == NULL) {
        fprintf(stderr, "Error stemming word '%s'.\n", word);
        free(stemmed_word);
        sb_stemmer_delete(stemmer);
        return NULL;
    }

	strcpy(stemmed_word, (char *)stemmed);

    sb_stemmer_delete(stemmer);
    return stemmed_word;
}