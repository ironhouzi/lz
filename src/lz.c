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

int min(const int x, const int y, const int z)
{
    int result = x;
    result = (x < y) ? x : y;
    result = (result < z) ? result : z;
    /* printf("x: %d, y: %d, z: %d -> %d\n", x, y, z, result); */
    /* printf("%d\n", result); */
    return result;
}

int ldist(String* str_s, String* str_t)
{
    int len = (str_s->length < str_t->length) ? str_s->length : str_t->length;
    if (strncmp(str_s->string, str_t->string, len) == 0)
        return 0;

    if (str_s->length == 0)
        return str_t->length;

    if (str_t->length == 0)
        return str_s->length;

    /* int s_len = str_s->length + 1; */
    /* int t_len = str_t->length + 1; */
    int s_len = str_s->length;
    int t_len = str_t->length;
    int s_vect[s_len];
    int t_vect[t_len];
    int i, j, cost;
    char chr_s;
    char chr_t;

    /* t_vect[t_len] = 0; */
    for (i = 0; i <= s_len + 1; i++)
        s_vect[i] = i;

    for (i = 0; i < t_len + 1; i++)
        t_vect[i] = 0;

    for (i = 0; i < s_len; i++) {
        t_vect[0] = i + 1;

        for (j = 0; j < t_len; j++) {
            chr_s = str_s->string[i];
            chr_t = str_t->string[j];
            cost = (strncmp(&chr_s, &chr_t, sizeof(char)) == 0) ? 0 : 1;
            /* printf("%c == %c, cost: ", chr_s, chr_t); */
            t_vect[j+1] = min(t_vect[j] + 1,
                              s_vect[j+1] + 1,
                              s_vect[j] + cost);
        }
        /* printf("\n"); */

        for (j = 0; j < s_len + 1; j++)
            s_vect[j] = t_vect[j];
    }

    return t_vect[t_len];
}

int main()
{
    String** strings = malloc(100 * sizeof(String*));
    int i;
    int wc = 0;
    /* String* i1 = make_string("Saturday"); */
    /* String* i2 = make_string("Sunday"); */
    /* String* i1 = make_string("kitten"); */
    /* String* i2 = make_string("sitting"); */
    char *str;

    string_from_stdin(strings, &wc);

    /* printf("input: %s\n", i1->string); */
    /* printf("input: %s\n", i2->string); */
    /* printf("dist: %d\n", ldist(i1, i2)); */
    for (i = 0; i < wc; i++) {
        str = strings[i]->string;
        printf("%s - %d\n", str, ldist(input, strings[i]));
    }

    free_all(strings, wc);
    return 0;
}
