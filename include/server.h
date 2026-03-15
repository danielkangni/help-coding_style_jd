/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** Server header - types and prototypes
*/

#ifndef SERVER_H
    #define SERVER_H

    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <sys/types.h>
    #include <sys/wait.h>
    #include <sys/stat.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <string.h>
    #include <poll.h>
    #include <errno.h>
    #include <ctype.h>
    #include <signal.h>
    #include <limits.h>
    #include <strings.h>

enum {
    MAX_PENDING = 128,
    MAX_CLIENTS = 150,
    BUF_SIZE = 4096
};

typedef struct client_s {
    int fd;
    int logged_in;
    int awaiting_pass;
    char user[150];
    char buf[BUF_SIZE];
    int buf_used;
    int pasv_listen_fd;
    int port_mode;
    struct sockaddr_in port_addr;
    char cwd[1024];
} client_t;

typedef struct pasv_info_s {
    int fd;
    char ipstr[64];
    int port;
} pasv_info_t;

extern client_t clients[MAX_CLIENTS];
extern char *home_path;
extern struct pollfd *g_fds;
extern int *g_nfds;

void send_reply(int fd, const char *msg);
int create_server_socket(int port);
void init_clients(struct pollfd *fds, int sockfd);
void accept_client(int sockfd,
    struct pollfd *fds, int *nfds);
void cleanup_client(int i,
    struct pollfd *fds, int *nfds);
void handle_clients(struct pollfd *fds, int *nfds);
void dispatch_command(int i, const char *cmd,
    const char *arg);
void cmd_user(int i, const char *arg);
void cmd_pass(int i, const char *arg);
void cmd_quit(int i);
void cmd_noop(int i);
void cmd_help(int i);
void cmd_pwd(int i);
void cmd_cwd(int i, const char *arg);
void cmd_pasv(int i);
void cmd_port(int i, const char *arg);
void cmd_list(int i);
int create_pasv_socket(pasv_info_t *info, int ctrl_fd);
void close_other_fds(int keep_ctrl, int keep_pasv);
int open_data_conn(int idx);
void start_list_transfer(int idx);

#endif
