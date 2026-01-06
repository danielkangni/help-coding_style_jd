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

typedef struct string {
    char *str;
    size_t size;
    bool (*empty)(const struct string *this);
    size_t (*length)(const struct string *this);
    char (*at)(const struct string *this, size_t pos);
    void (*print)(const struct string *this);
    const char *(*data)(const struct string *this);
    void (*clear)(struct string *this);
    void (*assign)(struct string *this, const char *str);
    void (*append)(struct string *this, const char *str);
} string_t;

void string_init(string_t *this, const char *s);
void string_destroy(string_t *this);
bool empty(const string_t *this);
size_t length (const string_t *this);
char at(const string_t *this , size_t pos);
void print(const string_t *this);
const char *data(const string_t *this);
void clear(string_t *this);
void assign (string_t *this, const char *str);
void append(string_t *this , const char *str);
char *my_strncpy(char *dest, const char *src, int n);
char *my_strdup(const char *str);
char *my_strcat(char *dest, const char *src);
int my_strlen(const char *s);
#endif