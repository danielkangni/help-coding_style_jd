/*
** EPITECH PROJECT, 2026
** my_strdup.c
** File description:
** my_strdup.c
*/

#include <string.h>
#include <stdlib.h>
#include "string.h"

char *my_strncpy(char *dest, const char *src, int n)
{
    int i = 0;

    if (!dest || !src)
        return NULL;
    while (i < n && src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    while (i < n) {
        dest[i] = '\0';
        i++;
    }
    return dest;
}

char *my_strdup(const char *str)
{
    int len = 0;
    char *new_str = NULL;

    if (str == NULL)
        return NULL;
    len = strlen(str) + 1;
    new_str = malloc(len);
    if (new_str == NULL)
        return NULL;
    my_strncpy(new_str, str, len);
    new_str[len - 1] = '\0';
    return new_str;
}

char *my_strcat(char *dest, const char *src)
{
    char *original_dest;

    if (!dest || !src)
        return NULL;
    original_dest = dest;
    while (*dest)
        dest++;
    while (*src) {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
    return original_dest;
}

int my_strlen(const char *s)
{
    int length = 0;

    if (!s)
        return 0;
    while (s[length] != '\0')
        length++;
    return length;
}
