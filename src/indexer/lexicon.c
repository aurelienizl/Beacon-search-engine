#include "lexicon.h"

void normalize(char *word) {
    for (int i = 0; word[i] != '\0'; i++) {
        word[i] = tolower(word[i]);
    }
}

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

int check_word(char *word) {
  struct sb_stemmer *stemmer = sb_stemmer_new("english", 0);
  if (!stemmer) {
    printf("Error creating stemmer\n");
    return 1;
  }
  
  const char *stop_words[] = {"a", "an", "the", "and", "but", "or", "not", "in", "on", "at", "to", "of", "for", "with", "by", "from", "as", "is", "am", "are", "was", "were", "be", "been", "being", "this", "that", "these", "those", "which", "who", "whom", "whose", "where", "when", "why", "how", "all", "any", "both", "each", "every", "few", "many", "more", "most", "other", "some", "such", "no", "nor", "not", "only", "own", "same", "so", "than", "too", "very", "s", "t", "can", "will", "just", "don", "should", "now"};
  int num_stop_words = sizeof(stop_words) / sizeof(stop_words[0]);
  
  int word_length = strlen(word);
  sb_symbol *stemmed_word = sb_stemmer_stem(stemmer, (const sb_symbol *)word, word_length);
  if (!stemmed_word) {
    printf("Error stemming word\n");
    return 1;
  }
  
  int is_stop_word = 0;
  for (int i = 0; i < num_stop_words; i++) {
    if (strcmp(stop_words[i], (const char *)stemmed_word) == 0) {
      is_stop_word = 1;
      break;
    }
  }
  
  sb_stemmer_delete(stemmer);

  if (is_stop_word) {
    return 1;
  } else {
    return 0;
  }  
}