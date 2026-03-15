/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** Navigation and transfer mode commands
*/

#include "../include/server.h"

void cmd_pwd(int i)
{
    char reply[1200];

    if (!clients[i].logged_in) {
        send_reply(clients[i].fd,
            "530 Not logged in.\r\n");
        return;
    }
    snprintf(reply, sizeof(reply),
        "257 \"%s\" is the current directory.\r\n",
        clients[i].cwd);
    send_reply(clients[i].fd, reply);
}

static void resolve_cwd(int i, const char *newpath)
{
    struct stat st;
    char resolved[PATH_MAX];

    if (stat(newpath, &st) != 0
        || !S_ISDIR(st.st_mode)) {
        send_reply(clients[i].fd,
            "550 Failed to change directory.\r\n");
        return;
    }
    if (realpath(newpath, resolved) != NULL)
        strncpy(clients[i].cwd, resolved,
            sizeof(clients[i].cwd) - 1);
    else
        strncpy(clients[i].cwd, newpath,
            sizeof(clients[i].cwd) - 1);
    clients[i].cwd[sizeof(clients[i].cwd) - 1] = '\0';
    send_reply(clients[i].fd,
        "250 Directory successfully changed.\r\n");
}

void cmd_cwd(int i, const char *arg)
{
    char newpath[2048];

    if (!clients[i].logged_in) {
        send_reply(clients[i].fd,
            "530 Not logged in.\r\n");
        return;
    }
    if (arg[0] == '/')
        snprintf(newpath, sizeof(newpath),
            "%s%s", home_path, arg);
    else
        snprintf(newpath, sizeof(newpath),
            "%s/%s", clients[i].cwd, arg);
    resolve_cwd(i, newpath);
}

void cmd_pasv(int i)
{
    pasv_info_t info;
    char reply[200];

    if (create_pasv_socket(&info,
        clients[i].fd) != 0) {
        send_reply(clients[i].fd,
            "425 Can't open passive connection.\r\n");
        return;
    }
    clients[i].pasv_listen_fd = info.fd;
    clients[i].port_mode = 0;
    snprintf(reply, sizeof(reply),
        "227 Entering Passive Mode (%s,%u,%u).\r\n",
        info.ipstr,
        info.port / 256, info.port % 256);
    send_reply(clients[i].fd, reply);
}
