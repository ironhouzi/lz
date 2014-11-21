#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "suffix.h"

void generate_suffixes(Word* word)
{
    int i, s;
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
    for (int i = 0; i < arraylen; i++) {
        free(all_words[i]->suffixes);
        free(all_words[i]->string);
        free(all_words[i]);
    }

    free(all_words);
}

void read_stdin(Word** words, int* wc)
{
    char buf[1024];
    int len;

    while(NULL != fgets(buf, 1024, stdin)) {
        len = strlen(buf) - 1;

        if (buf[len] == '\n')
            buf[len] = '\0';
        words[*wc] = make_word(buf);
        *wc += 1;
    }
}

int main()
{
    Word** words = malloc(100 * sizeof(Word*));
    int i, j;
    int wc = 0;

    read_stdin(words, &wc);

    for (i = 0; i < wc; i++) {
        printf("%s\n", words[i]->string);
        for (j = 0; j < words[i]->suffixcount; j++) {
            printf("%s\n", words[i]->suffixes[j]);
        }
    }

    free_all(words, wc);
    return 0;
}
