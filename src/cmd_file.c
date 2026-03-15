/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** DELE, RETR and STOR commands
*/

#include "../include/server.h"

static void build_path(int i, const char *arg, char *out)
{
    if (arg[0] == '/')
        snprintf(out, 2048, "%s%s",
            get_home_path(), arg);
    else
        snprintf(out, 2048, "%s/%s",
            clients[i].cwd, arg);
}

void cmd_dele(int i, const char *arg)
{
    char path[2048];

    if (!clients[i].logged_in) {
        send_reply(clients[i].fd,
            "530 Not logged in.\r\n");
        return;
    }
    build_path(i, arg, path);
    if (unlink(path) == 0)
        send_reply(clients[i].fd,
            "250 Delete operation successful.\r\n");
    else
        send_reply(clients[i].fd,
            "550 Delete operation failed.\r\n");
}

void cmd_retr(int i, const char *arg)
{
    char path[2048];

    if (!clients[i].logged_in) {
        send_reply(clients[i].fd,
            "530 Not logged in.\r\n");
        return;
    }
    build_path(i, arg, path);
    start_retr_transfer(i, path);
}

void cmd_stor(int i, const char *arg)
{
    char path[2048];

    if (!clients[i].logged_in) {
        send_reply(clients[i].fd,
            "530 Not logged in.\r\n");
        return;
    }
    build_path(i, arg, path);
    start_stor_transfer(i, path);
}
