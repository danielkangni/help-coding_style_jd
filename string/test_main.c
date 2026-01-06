/*
** EPITECH PROJECT, 2025
** string.c
** Implementation of string_t methods
*/
#include <stdio.h>
#include "string.h"

int main (void)
{
    string_t s;

    string_init(&s , "Foo");
    s.append(&s , "Bar\n");
    s.print(&s);
    s.clear(&s);
    s.append(&s , "Hello");
    s.append(&s , " ");
    s.append(&s , "World \n");
    s.print(&s);
    string_destroy(&s);
    return (0) ;
}
