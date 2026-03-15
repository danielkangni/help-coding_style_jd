/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** Data connection helpers
*/

#include "../include/server.h"

void close_other_fds(int keep_ctrl, int keep_pasv)
{
    int j;

    for (j = 1; j < MAX_CLIENTS; j++) {
        if (clients[j].fd != -1
            && clients[j].fd != keep_ctrl)
            close(clients[j].fd);
        if (clients[j].pasv_listen_fd != -1
            && clients[j].pasv_listen_fd != keep_pasv)
            close(clients[j].pasv_listen_fd);
    }
}

static int open_port_conn(int idx)
{
    int fd;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
        return (-1);
    if (connect(fd,
        (struct sockaddr *)&clients[idx].port_addr,
        sizeof(clients[idx].port_addr)) < 0) {
        close(fd);
        return (-1);
    }
    return (fd);
}

int open_data_conn(int idx)
{
    struct sockaddr_in peer;
    socklen_t len = sizeof(peer);
    int fd;

    if (clients[idx].port_mode)
        return (open_port_conn(idx));
    fd = accept(clients[idx].pasv_listen_fd,
        (struct sockaddr *)&peer, &len);
    close(clients[idx].pasv_listen_fd);
    clients[idx].pasv_listen_fd = -1;
    return (fd);
}
