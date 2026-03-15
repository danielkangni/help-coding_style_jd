/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** Main entry point and server loop
*/

#include "../include/server.h"

struct pollfd *g_fds = NULL;
int *g_nfds = NULL;

static void main_loop(int sockfd,
    struct pollfd *fds, int *nfds)
{
    while (1) {
        while (waitpid(-1, NULL, WNOHANG) > 0);
        if (poll(fds, *nfds, -1) == -1)
            continue;
        if (fds[0].revents & POLLIN)
            accept_client(sockfd, fds, nfds);
        handle_clients(fds, nfds);
    }
}

int main(int ac, char **av)
{
    struct pollfd fds[MAX_CLIENTS];
    int nfds = 1;
    int sockfd;

    if (ac != 3) {
        printf("USAGE: ./myftp port path\n");
        return (0);
    }
    signal(SIGPIPE, SIG_IGN);
    home_path = av[2];
    sockfd = create_server_socket(atoi(av[1]));
    if (sockfd < 0)
        return (1);
    g_fds = fds;
    g_nfds = &nfds;
    init_clients(fds, sockfd);
    main_loop(sockfd, fds, &nfds);
    return (0);
}
