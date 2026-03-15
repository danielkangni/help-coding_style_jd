/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** PORT and LIST commands
*/

#include "../include/server.h"

static void fill_port_addr(int i, int *v)
{
    char ipstr[32];

    snprintf(ipstr, sizeof(ipstr),
        "%d.%d.%d.%d", v[0], v[1], v[2], v[3]);
    clients[i].port_mode = 1;
    clients[i].pasv_listen_fd = -1;
    memset(&clients[i].port_addr, 0,
        sizeof(clients[i].port_addr));
    clients[i].port_addr.sin_family = AF_INET;
    clients[i].port_addr.sin_addr.s_addr =
        inet_addr(ipstr);
    clients[i].port_addr.sin_port =
        htons(v[4] * 256 + v[5]);
    send_reply(clients[i].fd,
        "200 PORT command successful.\r\n");
}

void cmd_port(int i, const char *arg)
{
    int v[6];

    if (sscanf(arg, "%d,%d,%d,%d,%d,%d",
        &v[0], &v[1], &v[2], &v[3],
        &v[4], &v[5]) != 6) {
        send_reply(clients[i].fd,
            "501 Syntax error in parameters.\r\n");
        return;
    }
    fill_port_addr(i, v);
}

void cmd_list(int i)
{
    if (!clients[i].logged_in) {
        send_reply(clients[i].fd,
            "530 Not logged in.\r\n");
        return;
    }
    start_list_transfer(i);
}
