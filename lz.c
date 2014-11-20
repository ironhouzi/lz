#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "lz.h"

void generate_suffixes(Word* word)
{
    size_t i, s;
    for(i = 0, s = 1; i < word->suffixcount; i++, s++) {
        word->suffixes[i] = word->string + s;
    }
}

Word* make_word(char* string) {
    int word_length = strlen(string);
    Word* word = malloc(sizeof(Word));
    word->suffixcount = word_length-1;
    word->string = malloc(word_length + 1);
    word->suffixes = malloc(word->suffixcount * sizeof(char*));
    strncpy(word->string, string, word_length + 1);
    generate_suffixes(word);

    return word;
}

void free_all(Word** all_words, int arraylen)
{
    int i;
    for (i = 0; i < arraylen; i++) {
        free(all_words[i]->suffixes);
        free(all_words[i]->string);
        free(all_words[i]);
    }

    free(all_words);
}

int main()
{
    char buf[1024];
    Word** words = malloc(100 * sizeof(Word*));
    int n = 0;
    size_t len;
    int i;
    size_t j;


    while(NULL != fgets(buf, 1024, stdin)) {
        len = strlen(buf) - 1;

        if (buf[len] == '\n')
            buf[len] = '\0';
        words[n++] = make_word(buf);
    }

    for (i = 0; i < n; i++) {
        printf("%s\n", words[i]->string);
        for (j = 0; j < words[i]->suffixcount; j++) {
            printf("%s\n", words[i]->suffixes[j]);
        }
    }

    free_all(words, n);
    return 0;
}
