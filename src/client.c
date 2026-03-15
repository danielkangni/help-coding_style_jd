/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** Client connection management
*/

#include "../include/server.h"

client_t clients[MAX_CLIENTS];
static char *home_path;

void set_home_path(char *path)
{
    home_path = path;
}

const char *get_home_path(void)
{
    return home_path;
}

static void register_client(int i, int fd,
    struct pollfd *fds, int *nfds)
{
    clients[i].fd = fd;
    clients[i].logged_in = 0;
    clients[i].awaiting_pass = 0;
    clients[i].buf_used = 0;
    clients[i].pasv_listen_fd = -1;
    clients[i].port_mode = 0;
    strncpy(clients[i].cwd, home_path,
        sizeof(clients[i].cwd) - 1);
    clients[i].cwd[sizeof(clients[i].cwd) - 1] = '\0';
    fds[i].fd = fd;
    fds[i].events = POLLIN;
    if (i >= *nfds)
        *nfds = i + 1;
    send_reply(fd, "220 Service ready for new user.\r\n");
}

void init_clients(struct pollfd *fds, int sockfd)
{
    for (int i = 0; i < MAX_CLIENTS; i++) {
        fds[i].fd = -1;
        fds[i].events = 0;
        clients[i].fd = -1;
        clients[i].buf_used = 0;
        clients[i].pasv_listen_fd = -1;
    }
    fds[0].fd = sockfd;
    fds[0].events = POLLIN;
}

void accept_client(int sockfd,
    struct pollfd *fds, int *nfds)
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    int fd = accept(sockfd,
        (struct sockaddr *)&addr, &len);

    if (fd == -1)
        return;
    for (int i = 1; i < MAX_CLIENTS; i++) {
        if (clients[i].fd != -1)
            continue;
        register_client(i, fd, fds, nfds);
        return;
    }
    send_reply(fd, "421 Too many users.\r\n");
    close(fd);
}

void cleanup_client(int i,
    struct pollfd *fds, int *nfds)
{
    close(clients[i].fd);
    if (clients[i].pasv_listen_fd != -1)
        close(clients[i].pasv_listen_fd);
    clients[i].fd = -1;
    clients[i].buf_used = 0;
    clients[i].logged_in = 0;
    clients[i].awaiting_pass = 0;
    clients[i].pasv_listen_fd = -1;
    clients[i].port_mode = 0;
    fds[i].fd = -1;
    while (*nfds > 1 && fds[*nfds - 1].fd == -1)
        (*nfds)--;
}
