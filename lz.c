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

String* make_string(char* string)
{
    String* str = malloc(sizeof(String));
    str->length = strlen(string);
    // '\0' isn't needed due to str->length
    str->string = malloc(str->length);
    strncpy(str->string, string, str->length);
    return str;
}

void del_string(String* str)
{
    free(str->string);
    free(str);
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

void read_stdin(char* buf, Word** words, int* wc)
{
    size_t len;
    while(NULL != fgets(buf, 1024, stdin)) {
        len = strlen(buf) - 1;

        if (buf[len] == '\n')
            buf[len] = '\0';
        words[*wc] = make_word(buf);
        *wc += 1;
    }
}

int ldist(String* str_s, String* str_t)
{
    size_t s_len = str_s->length + 1;
    size_t t_len = str_t->length + 1;
    int v0[s_len];
    int v1[t_len];
    int i, j;

    for (i = 0; i < s_len; i++)
        v0[i] = i;

    for (i = 0; i < str_s->length; i++) {
        for (j = 0; j < str_s->length; j++) {
        }
    }
}

int main()
{
    char buf[1024];
    Word** words = malloc(100 * sizeof(Word*));
    int i;
    int wc = 0;
    size_t j;

    read_stdin(buf, words, &wc);

    for (i = 0; i < wc; i++) {
        printf("%s\n", words[i]->string);
        for (j = 0; j < words[i]->suffixcount; j++) {
            printf("%s\n", words[i]->suffixes[j]);
        }
    }

    free_all(words, wc);
    return 0;
}
