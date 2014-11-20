typedef struct _suffix {
    size_t suffixcount;
    char* string;
    char** suffixes;
} Word;

Word* make_word(char* string);
