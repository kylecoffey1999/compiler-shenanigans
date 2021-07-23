#ifndef MYSTRING_H
#define MYSTRING_H

typedef struct
{
    char *buf;
    int len;
} string_t;

string_t *string_new(void);
string_t *string_append(string_t *str, const char *begin, const char *end);

#endif // MYSTRING_H
