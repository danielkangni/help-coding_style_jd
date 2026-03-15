/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** Command parsing and client I/O
*/

#include "../include/server.h"

void send_reply(int fd, const char *msg)
{
    write(fd, msg, strlen(msg));
}

static void parse_command(const char *line,
    char *cmd, char *arg)
{
    int i = 0;
    int j = 0;

    while (line[i] && line[i] != ' ' && i < 15) {
        cmd[i] = toupper(line[i]);
        i++;
    }
    cmd[i] = '\0';
    if (line[i] == ' ')
        i++;
    while (line[i]) {
        arg[j] = line[i];
        i++;
        j++;
    }
    arg[j] = '\0';
}

static void process_line(int i, char *line)
{
    char cmd[16] = {0};
    char arg[BUF_SIZE] = {0};
    int len = strlen(line);

    while (len > 0 && (line[len - 1] == '\r'
        || line[len - 1] == '\n')) {
        line[len - 1] = '\0';
        len--;
    }
    parse_command(line, cmd, arg);
    dispatch_command(i, cmd, arg);
}

static void process_buffer(int i)
{
    char *nl;

    while ((nl = strstr(clients[i].buf, "\r\n"))
        != NULL) {
        *nl = '\0';
        process_line(i, clients[i].buf);
        if (clients[i].fd == -1)
            return;
        nl += 2;
        clients[i].buf_used -= (nl - clients[i].buf);
        memmove(clients[i].buf, nl, clients[i].buf_used);
        clients[i].buf[clients[i].buf_used] = '\0';
    }
}

void handle_clients(struct pollfd *fds, int *nfds)
{
    int r;
    int i;

    for (i = 1; i < *nfds; i++) {
        if (fds[i].fd == -1 || !(fds[i].revents & POLLIN))
            continue;
        r = read(fds[i].fd, clients[i].buf
            + clients[i].buf_used,
            BUF_SIZE - clients[i].buf_used - 1);
        if (r <= 0) {
            cleanup_client(i, fds, nfds);
            continue;
        }
        clients[i].buf_used += r;
        clients[i].buf[clients[i].buf_used] = '\0';
        process_buffer(i);
    }
}
