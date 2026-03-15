/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** Passive socket creation
*/

#include "../include/server.h"

static void get_local_ip(int ctrl_fd,
    char *ipstr, int iplen)
{
    struct sockaddr_in local;
    socklen_t len = sizeof(local);
    unsigned char *b;

    if (getsockname(ctrl_fd,
        (struct sockaddr *)&local, &len) < 0) {
        strncpy(ipstr, "127,0,0,1", iplen);
        return;
    }
    b = (unsigned char *)&local.sin_addr.s_addr;
    snprintf(ipstr, iplen,
        "%u,%u,%u,%u", b[0], b[1], b[2], b[3]);
}

static int bind_listen_socket(void)
{
    struct sockaddr_in sa = {0};
    int sd = socket(AF_INET, SOCK_STREAM, 0);

    if (sd < 0)
        return (-1);
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = htonl(INADDR_ANY);
    sa.sin_port = htons(0);
    if (bind(sd, (struct sockaddr *)&sa,
        sizeof(sa)) < 0 || listen(sd, 1) < 0) {
        close(sd);
        return (-1);
    }
    return (sd);
}

int create_pasv_socket(pasv_info_t *info, int ctrl_fd)
{
    struct sockaddr_in bound;
    socklen_t len = sizeof(bound);
    int sd = bind_listen_socket();

    if (sd < 0)
        return (-1);
    if (getsockname(sd,
        (struct sockaddr *)&bound, &len) < 0) {
        close(sd);
        return (-1);
    }
    get_local_ip(ctrl_fd, info->ipstr,
        sizeof(info->ipstr));
    info->fd = sd;
    info->port = ntohs(bound.sin_port);
    return (0);
}
