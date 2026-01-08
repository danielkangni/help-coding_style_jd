/*
** EPITECH PROJECT, 2026
** my_strdup.c
** File description:
** my_strdup.c
*/

#include "array.h"

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

void bind_methods(array_t *this)
{
    this->empty = &empty;
    this->size = &size;
    this->resize = &resize;
    this->get = &get;
    this->set = &set;
    this->apply = &apply;
}