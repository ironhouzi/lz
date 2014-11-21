typedef struct _suffix {
    size_t suffixcount;
    char* string;
    char** suffixes;
} Word;

typedef struct _string {
    size_t length;
    char *string;
} String;

Word* make_word(char* string);
String* make_string(char* string);
