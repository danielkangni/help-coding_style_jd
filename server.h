#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <poll.h>
#include <errno.h>
#include <ctype.h>

#define MAX_PENDING 5
#define MAX_CLIENTS 64
#define BUF_SIZE    4096

typedef struct client_s {
    int fd;
    int logged_in;
    int awaiting_pass;
    char user[64];
    char buf[BUF_SIZE];
    int buf_used;
    int pasv_listen_fd;
    int port_mode;
    struct sockaddr_in port_addr;
    char cwd[1024];
} client_t;

extern client_t clients[MAX_CLIENTS];
extern char    *home_path;

void send_reply(int fd, const char *msg);

int  create_server_socket(int port);

void init_clients(struct pollfd *fds, int sockfd);
void accept_client(int sockfd, struct pollfd *fds, int *nfds);
void cleanup_client(int i, struct pollfd *fds, int *nfds);

void dispatch_command(int i, const char *cmd, const char *arg,
                      struct pollfd *fds, int *nfds);

int  create_pasv_socket(int *out_fd, char *ipstr, int iplen,
    int *out_port, int ctrl_fd);
void start_list_transfer(int idx);

#endif
