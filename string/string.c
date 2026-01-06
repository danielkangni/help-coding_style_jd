/*
** EPITECH PROJECT, 2025
** string.c
** Implementation of string_t methods
*/

#include "string.h"

void string_init(string_t *this, const char *s)
{
    if (!this)
        return;
    this->str = NULL;
    this->size = 0;
    if (s) {
        this->str = my_strdup(s);
        this->size = (size_t) my_strlen(s);
    }
    this->empty = empty;
    this->length = length;
    this->at = at;
    this->print = print;
    this->data = data;
    this->clear = clear;
    this->assign = assign;
    this->append = append;
}

void string_destroy(string_t *this)
{
    if (!this)
        return;
    free(this->str);
    this->str = NULL;
    this->size = 0;
}

size_t length(const string_t *this)
{
    if (!this)
        return 0;
    return this->size;
}

bool empty(const string_t *this)
{
    if (!this)
        return true;
    return this->size == 0;
}

char at(const string_t *this, size_t pos)
{
    if (!this || pos >= this->size)
        return '\0';
    return this->str[pos];
}

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