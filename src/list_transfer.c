/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** LIST data transfer
*/

#include "../include/server.h"

static void send_listing(int data_fd, const char *cwd)
{
    char cmd[1100];
    char line[1024];
    FILE *ls;

    snprintf(cmd, sizeof(cmd), "ls -la %s", cwd);
    ls = popen(cmd, "r");
    if (!ls)
        return;
    while (fgets(line, sizeof(line), ls))
        write(data_fd, line, strlen(line));
    pclose(ls);
}

static void child_list(int idx)
{
    int ctrl_fd = clients[idx].fd;
    int pasv_fd = clients[idx].pasv_listen_fd;
    int data_fd;

    close_other_fds(ctrl_fd, pasv_fd);
    data_fd = open_data_conn(idx);
    if (data_fd < 0) {
        send_reply(ctrl_fd,
            "425 Can't open data connection.\r\n");
        _exit(1);
    }
    send_listing(data_fd, clients[idx].cwd);
    close(data_fd);
    send_reply(ctrl_fd,
        "226 Directory send OK.\r\n");
    _exit(0);
}

static void parent_cleanup(int idx)
{
    if (!clients[idx].port_mode
        && clients[idx].pasv_listen_fd != -1) {
        close(clients[idx].pasv_listen_fd);
        clients[idx].pasv_listen_fd = -1;
    }
}

void start_list_transfer(int idx)
{
    pid_t pid;
    int ctrl_fd = clients[idx].fd;

    if (clients[idx].port_mode == 0
        && clients[idx].pasv_listen_fd == -1) {
        send_reply(ctrl_fd,
            "425 Use PORT or PASV first.\r\n");
        return;
    }
    send_reply(ctrl_fd,
        "150 Here comes the directory listing.\r\n");
    pid = fork();
    if (pid < 0) {
        send_reply(ctrl_fd,
            "451 Requested action aborted.\r\n");
        return;
    }
    if (pid == 0)
        child_list(idx);
    parent_cleanup(idx);
}
