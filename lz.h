typedef struct _string {
    int length;
    char *string;
} String;

String* make_string(char* string);
