/*
** EPITECH PROJECT, 2025
** my.h
** File description:
** A .h file to use my library libmy.a
*/

#ifndef STRING_H
    #define STRING_H
    #include <stdarg.h>
    #include <stdio.h>
    #include <sys/stat.h>
    #include <stdbool.h>
    #include <errno.h>
    #include <stdlib.h>
    #include <dirent.h>
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <fcntl.h>
    #include <string.h>
    #include <ctype.h>

typedef struct array {
    void **str;
    size_t _size;
    bool (*empty)(const struct array *thi);
    size_t (*size)(const struct array *thi);
    void (*resize)(struct array *thi, size_t _size);
    void *(*get)(const struct array *thi, size_t index);
    bool (*set)(struct array *thi, size_t index, void *value);
    void (*apply)(struct array *thi, void *(*function)(void *));
} array_t;

void array_init(array_t *thi, size_t size);
void array_destroy(array_t *thi);
bool empty(const array_t *thi);
size_t size(const array_t *thi);
void resize(array_t *thi, size_t size);
void bind_methods(array_t *this);
void *get(const array_t *thi, size_t index);
bool set(array_t *thi, size_t index, void *value);
void apply(array_t *thi, void *(*function)(void *));
char *my_strncpy(char *dest, const char *src, int n);
char *my_strdup(const char *str);
char *my_strcat(char *dest, const char *src);
int my_strlen(const char *s);
void reset_array(array_t *this);
void fill_null(void **data, size_t size, size_t start);
void **alloc_data(size_t size);
void shrink_to_zero(array_t *this);
void copy_old_into_new(array_t *this, void **new_data, size_t new_size);
#endif
