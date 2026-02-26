#include "server.h"

client_t clients[MAX_CLIENTS];
char    *home_path = NULL;

void send_reply(int fd, const char *msg)
{
    write(fd, msg, strlen(msg));
}

static void parse_command(const char *line, char *cmd, char *arg)
{
    int i = 0;

    while (line[i] && line[i] != ' ' && i < 15) {
        cmd[i] = toupper((unsigned char)line[i]);
        i++;
    }
    cmd[i] = '\0';
    if (line[i] == ' ')
        strncpy(arg, line + i + 1, 511);
    arg[511] = '\0';
}

static void process_line(int i, char *line, struct pollfd *fds, int *nfds)
{
    char cmd[16] = {0};
    char arg[512] = {0};

    parse_command(line, cmd, arg);
    dispatch_command(i, cmd, arg, fds, nfds);
}

static void process_buffer(int i, struct pollfd *fds, int *nfds)
{
    char *p;
    char line[1024];
    size_t linelen;
    size_t consumed;

    while ((p = memchr(clients[i].buf, '\n', clients[i].buf_used))) {
        linelen = p - clients[i].buf;
        if (linelen >= sizeof(line)) linelen = sizeof(line) - 1;
        memcpy(line, clients[i].buf, linelen);
        if (linelen > 0 && line[linelen - 1] == '\r') linelen--;
        line[linelen] = '\0';
        consumed = (p - clients[i].buf) + 1;
        memmove(clients[i].buf, clients[i].buf + consumed,
            clients[i].buf_used - consumed);
        clients[i].buf_used -= consumed;
        process_line(i, line, fds, nfds);
        if (clients[i].fd == -1) break;
    }
}

static void process_client(int i, struct pollfd *fds, int *nfds)
{
    char readbuf[1024];
    int n = read(clients[i].fd, readbuf, sizeof(readbuf) - 1);

    if (n <= 0) {
        cleanup_client(i, fds, nfds);
        return;
    }
    readbuf[n] = '\0';
    if (clients[i].buf_used + n < BUF_SIZE) {
        memcpy(clients[i].buf + clients[i].buf_used, readbuf, n);
        clients[i].buf_used += n;
    } else {
        clients[i].buf_used = 0;
    }
    process_buffer(i, fds, nfds);
}

static void handle_clients(struct pollfd *fds, int *nfds)
{
    for (int i = 1; i < *nfds; i++) {
        if (clients[i].fd == -1) continue;
        if (!(fds[i].revents & POLLIN)) continue;
        process_client(i, fds, nfds);
    }
}

static void main_loop(int sockfd, struct pollfd *fds, int *nfds)
{
    while (1) {
        while (waitpid(-1, NULL, WNOHANG) > 0) {}
        if (poll(fds, *nfds, -1) == -1) {
            if (errno == EINTR) continue;
            continue;
        }
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
        return 0;
    }
    home_path = av[2];
    sockfd = create_server_socket(atoi(av[1]));
    if (sockfd < 0)
        return 1;
    init_clients(fds, sockfd);
    main_loop(sockfd, fds, &nfds);
    return 0;
}
