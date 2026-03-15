/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** Authentication and utility commands
*/

#include "../include/server.h"

void cmd_user(int i, const char *arg)
{
    clients[i].awaiting_pass = 1;
    strncpy(clients[i].user, arg,
        sizeof(clients[i].user) - 1);
    send_reply(clients[i].fd,
        "331 Please specify the password.\r\n");
}

void cmd_pass(int i, const char *arg)
{
    if (!clients[i].awaiting_pass) {
        send_reply(clients[i].fd,
            "503 Login with USER first.\r\n");
        return;
    }
    if (strcasecmp(clients[i].user, "anonymous") == 0
        && strlen(arg) == 0) {
        clients[i].logged_in = 1;
        clients[i].awaiting_pass = 0;
        send_reply(clients[i].fd,
            "230 Login successful.\r\n");
    } else {
        clients[i].awaiting_pass = 0;
        send_reply(clients[i].fd,
            "530 Login incorrect.\r\n");
    }
}

void cmd_quit(int i)
{
    send_reply(clients[i].fd, "221 Goodbye.\r\n");
    cleanup_client(i, g_fds, g_nfds);
}

void cmd_noop(int i)
{
    send_reply(clients[i].fd, "200 NOOP ok.\r\n");
}

void cmd_help(int i)
{
    send_reply(clients[i].fd,
        "214-The following commands are recognized:\r\n"
        " USER PASS QUIT PWD CWD PASV PORT LIST"
        " NOOP HELP\r\n"
        "214 Help OK.\r\n");
}
