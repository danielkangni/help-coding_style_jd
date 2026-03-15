/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** RETR data transfer
*/

#include "../include/server.h"

static void send_file_data(int data_fd, int file_fd)
{
    char buf[4096];
    int r;

    r = read(file_fd, buf, sizeof(buf));
    while (r > 0) {
        write(data_fd, buf, r);
        r = read(file_fd, buf, sizeof(buf));
    }
}

static void do_retr(int ctrl_fd, int data_fd,
    const char *path)
{
    int file_fd;

    file_fd = open(path, O_RDONLY);
    if (file_fd < 0) {
        close(data_fd);
        send_reply(ctrl_fd,
            "550 Failed to open file.\r\n");
        _exit(1);
    }
    send_file_data(data_fd, file_fd);
    close(file_fd);
    close(data_fd);
    send_reply(ctrl_fd,
        "226 Transfer complete.\r\n");
    _exit(0);
}

static void child_retr(int idx, const char *path)
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
    do_retr(ctrl_fd, data_fd, path);
}

static void parent_cleanup(int idx)
{
    if (!clients[idx].port_mode
        && clients[idx].pasv_listen_fd != -1) {
        close(clients[idx].pasv_listen_fd);
        clients[idx].pasv_listen_fd = -1;
    }
}

void start_retr_transfer(int idx, const char *path)
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
        "150 Opening data connection.\r\n");
    pid = fork();
    if (pid < 0) {
        send_reply(ctrl_fd,
            "451 Requested action aborted.\r\n");
        return;
    }
    if (pid == 0)
        child_retr(idx, path);
    parent_cleanup(idx);
}
