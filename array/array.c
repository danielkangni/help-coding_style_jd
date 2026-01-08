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

void resize(array_t *this, size_t size)
{
    void **new_data = NULL;

    if (!this)
        return;
    if (size == 0)
        return shrink_to_zero(this);
    new_data = alloc_data(size);
    if (!new_data)
        return;
    if (this->str)
        copy_old_into_new(this, new_data, size);
    free(this->str);
    this->str = new_data;
    this->_size = size;
}

void array_init(array_t *this, size_t size)
{
    if (!this)
        return;
    reset_array(this);
    bind_methods(this);
    if (size == 0)
        return;
    this->str = alloc_data(size);
    if (!this->str)
        return;
    this->_size = size;
}

void array_destroy(array_t *this)
{
    if (!this)
        return;
    free(this->str);
    reset_array(this);
}