#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "lz.h"

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

void free_all(String** all_strings, int arraylen)
{
    for (int i = 0; i < arraylen; i++) {
        free(all_strings[i]->string);
        free(all_strings[i]);
    }

    free(all_strings);
}

void string_from_stdin(String** strings, int* wc)
{
    char buf[1024];
    int len;

    while(NULL != fgets(buf, 1024, stdin)) {
        len = strlen(buf) - 1;

        if (buf[len] == '\n')
            buf[len] = '\0';
        strings[*wc] = make_string(buf);
        *wc += 1;
    }
}

int ldist(String* str_s, String* str_t)
{
    int s_len = str_s->length + 1;
    int t_len = str_t->length + 1;
    int v0[s_len];
    int v1[t_len];
    int i, j, cost;

    for (i = 0; i < s_len; i++)
        v0[i] = i;

    for (i = 0; i < str_s->length; i++) {

        v1[0] = i + 1;

        for (j = 0; j < str_t->length; j++) {
            cost = 1;
            /* cost = (str_s[i] == str_t[j]) ? 0 : 1; */
            /* if (strncmp((const) str_s->string[i], (const) str_t->string[j], sizeof(char)) == 0) */
        }
    }

    return 1;
}

int main()
{
    /* Word** words = malloc(100 * sizeof(Word*)); */
    String** strings = malloc(100 * sizeof(String*));
    int i, j;
    int wc = 0;

    /* read_stdin(buf, words, &wc); */
    string_from_stdin(strings, &wc);

    for (i = 0; i < wc; i++) {
        for (j = 0; j < strings[i]->length; j++) {
            printf("%s\n", &(strings[i]->string[j]));
        }
    }


    /* for (i = 0; i < wc; i++) { */
    /*     printf("%s\n", words[i]->string); */
    /*     for (j = 0; j < words[i]->suffixcount; j++) { */
    /*         printf("%s\n", words[i]->suffixes[j]); */
    /*     } */
    /* } */

    free_all(strings, wc);
    /* free_all(words, wc); */
    return 0;
}
