/*
** EPITECH PROJECT, 2026
** string.c
** Implementation of string_t methods
** string.c
*/

#include "string.h"

void print(const string_t *this)
{
    if (!this || !this->str)
        return;
    printf("%s", this->str);
}

const char *data(const string_t *this)
{
    if (!this)
        return NULL;
    return this->str;
}

void clear(string_t *this)
{
    if (!this) {
        abort();
        return;
    }
    free(this->str);
    this->str = NULL;
    this->size = 0;
}

void assign(string_t *this, const char *str)
{
    if (!this) {
        abort();
        return;
    }
    clear(this);
    if (str) {
        this->str = my_strdup(str);
        this->size = (size_t) my_strlen(str);
    }
}

void append(string_t *this, const char *str)
{
    char *new_str;
    size_t add;

    if (!this || !str) {
        abort();
        return;
    }
    add = (size_t) my_strlen(str);
    new_str = malloc(this->size + add + 1);
    if (!new_str)
        return;
    new_str[0] = '\0';
    if (this->str)
        my_strcat(new_str, this->str);
    my_strcat(new_str, str);
    free(this->str);
    this->str = new_str;
    this->size += add;
}