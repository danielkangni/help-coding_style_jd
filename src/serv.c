/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** Server socket creation
*/

#include "../include/server.h"

static int bind_server(int fd, int port)
{
    struct sockaddr_in sa = {0};

    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = htonl(INADDR_ANY);
    sa.sin_port = htons(port);
    if (bind(fd, (struct sockaddr *)&sa,
        sizeof(sa)) != 0) {
        perror("bind");
        return (-1);
    }
    if (listen(fd, MAX_PENDING) == -1) {
        perror("listen");
        return (-1);
    }
    return (0);
}

int create_server_socket(int port)
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;

    if (fd < 0) {
        perror("socket");
        return (-1);
    }
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR,
        &opt, sizeof(opt));
    if (bind_server(fd, port) != 0)
        return (-1);
    return (fd);
}
