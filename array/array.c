/*
** EPITECH PROJECT, 2026
** my_strdup.c
** File description:
** my_strdup.c
*/

#include "array.h"

bool empty(const array_t *this)
{
    if (!this)
        return true;
    return this->_size == 0;
}

size_t size(const array_t *this)
{
    if (!this)
        return 0;
    return this->_size;
}

void *get(const array_t *this, size_t index)
{
    if (!this || !this->str || index >= this->_size)
        return NULL;
    return this->str[index];
}

bool set(array_t *this, size_t index, void *value)
{
    if (!this || !this->str || index >= this->_size)
        return false;
    this->str[index] = value;
    return true;
}

void apply(array_t *this, void *(*function)(void *))
{
    size_t i = 0;

    if (!this || !this->str || !function)
        return;
    while (i < this->_size) {
        this->str[i] = function(this->str[i]);
        ++i;
    }
}
