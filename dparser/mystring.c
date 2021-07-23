#include "mystring.h"
#include <stdlib.h>
#include <string.h>

string_t *string_new(void)
{
    string_t *str = malloc(sizeof(string_t));
    str->buf = NULL;
    str->len = 0;
    return str;
}

string_t *string_append(string_t *str, const char *begin, const char *end)
{
    str->buf = realloc(str->buf, str->len + end - begin);
    memcpy(str->buf + str->len, begin, end - begin);
    return str;
}
