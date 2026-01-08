/*
** EPITECH PROJECT, 2026
** my_strdup.c
** File description:
** my_strdup.c
*/
#include "array.h"
#include <stdlib.h>

void reset_array(array_t *this)
{
    if (!this)
        return;
    this->str = NULL;
    this->_size = 0;
}

void fill_null(void **data, size_t _size, size_t start)
{
    size_t i = start;

    while (i < _size) {
        data[i] = NULL;
        ++i;
    }
}

void **alloc_data(size_t _size)
{
    void **data = NULL;

    if (_size == 0)
        return NULL;
    data = malloc(sizeof(void *) * _size);
    if (!data)
        return NULL;
    fill_null(data, _size, 0);
    return data;
}

void shrink_to_zero(array_t *this)
{
    free(this->str);
    this->str = NULL;
    this->_size = 0;
}

void copy_old_into_new(array_t *this, void **new_data, size_t new__size)
{
    size_t i = 0;
    size_t limit = this->_size;

    if (limit > new__size)
        limit = new__size;
    while (i < limit) {
        new_data[i] = this->str[i];
        ++i;
    }
}
