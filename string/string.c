/*
** EPITECH PROJECT, 2026
** string.c
** Implementation of string_t methods
** string.c
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