#ifndef STR_H
#define STR_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>

/**
 * String struct that stores length and null-terminated data.
 */
typedef struct {
    size_t len;
    char *data;
} Str;

/* Utility functions */

/**
 * Reversed memcpy. Copies from src to dest in reverse order.
 */
static void *rev_memcpy(void *dest, const void *src, size_t len)
{
    char *d = (char *)dest + len - 1;
    const char *s = (char *)src;
    while (len--)
        *d-- = *s++;
    return dest;
}

/* Initialisation */

/**
 * Create a Str from raw data and length.
 */
Str str_create(const char *data, size_t len);

/**
 * Create a Str from a null-terminated C string.
 */
Str str_from_cstr(const char *data);

/* Manipulation */

/**
 * Concatenate two Strs into a new Str.
 */
Str str_concat(Str *s1, Str *s2);

/**
 * Append a character to the end of a Str.
 */
void str_append(Str *s, char c);

/**
 * Slice a substring from a Str between start and end.
 */
Str str_slice(const Str *s, int start, int end);

/**
 * Duplicate a Str (deep copy).
 */
Str str_dupl(Str *s);

/* Case transformations */

/**
 * Convert Str to uppercase (ASCII).
 */
void str_to_upper(Str *s);

/**
 * Convert Str to lowercase (ASCII).
 */
void str_to_lower(Str *s);

/**
 * Reverse the case of each character in a Str (ASCII).
 */
void str_reverse_case(Str *s);

/* Checks */

/**
 * Count the number of instances of a character.
 */
int str_instances_of(Str *s, char c);

/**
 * Check if Str is fully alphabetic.
 */
int str_is_alpha(Str *s);

/**
 * Check if Str contains only whitespace (space, tab, newline).
 */
int str_is_space(Str *s);

/**
 * Check if Str is a valid integer (with optional sign).
 */
int str_is_int(Str *s);

/**
 * Check if Str is a valid float (with optional 'f' suffix).
 */
int str_is_float(Str *s);

/* Conversion */

/**
 * Convert Str to int if valid. Writes result to `*n`.
 */
void str_to_int(Str *s, int *n);

/**
 * Placeholder: Convert Str to long (not implemented).
 */
long str_to_long(Str *s);

/**
 * Placeholder: Convert Str to float (not implemented).
 */
float str_to_float(Str *s);

/**
 * Placeholder: Convert Str to double (not implemented).
 */
double str_to_double(Str *s);

/* Memory Management */

/**
 * Internal: Frees any number of Str* objects. Must end with NULL.
 */
static void _str_free_all(Str *first, ...)
{
    va_list args;
    va_start(args, first);

    Str *s = first;
    while (s != NULL) {
        if (s->data)
            free(s->data);
        s->data = NULL;
        s->len = 0;
        s = va_arg(args, Str *);
    }

    va_end(args);
}

/**
 * Macro: Free any number of Str* arguments, automatically ends with NULL.
 */
#define str_free_all(...) _str_free_all(__VA_ARGS__, NULL)

#endif // STR_H

/* Implementation */
#ifdef STR_IMPLEMENTATION

Str str_create(const char *data, size_t len)
{
    char *init_data = malloc(len + 1); 
    if (!init_data) {
        fprintf(stderr, "failed string data initialisation\n");
        return (Str){.data = NULL, .len = 0};
    }

    memcpy(init_data, data, len);
    init_data[len] = '\0';

    return (Str){ .data = init_data, .len = len };
}

Str str_from_cstr(const char *data)
{
    size_t len = strlen(data);
    char *init_data = malloc(len + 1); 
    if (!init_data) {
        fprintf(stderr, "failed string data initialisation\n");
        return (Str){ .data = NULL, .len = len };
    }

    memcpy(init_data, data, len);
    init_data[len] = '\0';

    return (Str){ .data = init_data, .len = len };
}

Str str_concat(Str *s1, Str *s2)
{
    size_t new_len = s1->len + s2->len;
    char *new_data = malloc(new_len + 1);
    if (!new_data) {
        fprintf(stderr, "failed to allocate memory for string concatenation\n");
        return (Str){.data = NULL, .len = 0};
    }

    memcpy(new_data, s1->data, s1->len);
    memcpy(new_data + s1->len, s2->data, s2->len);
    new_data[new_len] = '\0';

    return (Str){ .data = new_data, .len = new_len };
}

void str_append(Str *s, char c)
{
    char tmp[2] = { c, '\0' };
    Str to_add = { .data = tmp, .len = 1 };
    Str new_s = str_concat(s, &to_add);

    free(s->data);
    *s = new_s;
}

Str str_slice(const Str *s, int start, int end)
{
    if (!s || !s->data) {
        fprintf(stderr, "missing s string for slicing\n");
        return (Str){ .data = NULL, .len = s ? s->len : 0 };
    }

    int new_len = end - start;
    if (new_len == 0 || (size_t)end > s->len) {
        fprintf(stderr, "invalid length for slicing\n");
        return (Str){ .data = NULL, .len = s ? s->len : 0 };
    }

    char *new_data = malloc(abs(new_len) + 1);
    if (!new_data) {
        fprintf(stderr, "failed allocation of new data for slice\n");
        return (Str){ .data = NULL, .len = s ? s->len : 0 };
    }

    if (new_len > 0)
        memcpy(new_data, s->data + start, abs(new_len));
    else
        rev_memcpy(new_data, s->data + end, abs(new_len));
    new_data[abs(new_len)] = '\0';

    return (Str){ .data = new_data, .len = abs(new_len) };
}

Str str_dupl(Str *s) {
    return str_create(s->data, s->len);
}

void str_to_upper(Str *s)
{
    if (!s || !s->data) return;
    for (size_t i = 0; i < s->len; i++)
        if (s->data[i] >= 'a' && s->data[i] <= 'z')
            s->data[i] -= 0x20;
}

void str_to_lower(Str *s)
{
    if (!s || !s->data) return;
    for (size_t i = 0; i < s->len; i++)
        if (s->data[i] >= 'A' && s->data[i] <= 'Z')
            s->data[i] += 0x20;
}

void str_reverse_case(Str *s)
{
    if (!s || !s->data) return;
    for (size_t i = 0; i < s->len; i++) {
        if (s->data[i] >= 'A' && s->data[i] <= 'Z')
            s->data[i] += 0x20;
        else if (s->data[i] >= 'a' && s->data[i] <= 'z')
            s->data[i] -= 0x20;
    }
}

int str_instances_of(Str *s, char c)
{
    if (!s || !s->data) return -1;
    int ctr = 0;
    for (size_t i = 0; i < s->len; i++)
        if (s->data[i] == c) ctr++;
    return ctr;
}

int str_is_alpha(Str *s)
{
    if (!s || !s->data) return -1;
    for (size_t i = 0; i < s->len; i++)
        if (!((s->data[i] >= 'A' && s->data[i] <= 'Z') || 
              (s->data[i] >= 'a' && s->data[i] <= 'z')))
            return 0;
    return 1;
}

int str_is_space(Str *s)
{
    if (!s || !s->data) return -1;
    for (size_t i = 0; i < s->len; i++)
        if (!(s->data[i] == ' ' || s->data[i] == '\t' || s->data[i] == '\n'))
            return 0;
    return 1;
}

int str_is_int(Str *s)
{
    if (!s || !s->data || s->len == 0) return -1;

    int start = (s->data[0] == '+' || s->data[0] == '-') ? 1 : 0;
    if (start == 1 && s->len == 1) return 0;

    for (size_t i = start; i < s->len; i++)
        if (s->data[i] < '0' || s->data[i] > '9')
            return 0;

    return 1;
}

int str_is_float(Str *s)
{
    if (!s || !s->data) return -1;

    if (str_instances_of(s, '.') != 1 || s->data[s->len - 1] == '.')
        return 0;

    int f_ctr = str_instances_of(s, 'f');
    if (f_ctr > 1 || (f_ctr == 1 && !(s->data[s->len - 1] == 'f')))
        return 0;

    int start = (s->data[0] == '+' || s->data[0] == '-') ? 1 : 0;
    if (s->len == 1 || str_instances_of(s, '+') > 1 || str_instances_of(s, '-') > 1)
        return 0;

    int digit_count = 0;
    for (size_t i = start; i < s->len; i++) {
        char c = s->data[i];
        if (c >= '0' && c <= '9')
            digit_count++;
        else if (c != '.' && c != 'f')
            return 0;
    }

    return digit_count > 0;
}

void str_to_int(Str *s, int *n) 
{
    if (!str_is_int(s)) {
        n = NULL;
        return;
    }

    int sign = 1;
    int num = 0;
    size_t i = 0;

    if (s->data[0] == '-') {
        sign = -1;
        i = 1;
    }

    for (; i < s->len; i++) {
        num = 10 * num + (s->data[i] - '0');
    }

    *n = num * sign;
}

long str_to_long(Str *s) { return 0; }
float str_to_float(Str *s) { return 0; }
double str_to_double(Str *s) { return 0; }

#endif // STR_IMPLEMENTATION
