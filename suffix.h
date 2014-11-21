typedef struct _suffix {
    int suffixcount;
    char* string;
    char** suffixes;
} Word;

void generate_suffixes(Word* word);
Word* make_word(char* string);
void free_all(Word** all_words, int arraylen);
void read_stdin(Word** words, int* wc);
