#include "server.h"

static void get_local_ip(int ctrl_fd, char *ipstr, int iplen)
{
    struct sockaddr_in local;
    socklen_t len = sizeof(local);
    unsigned char *b;

    if (getsockname(ctrl_fd, (struct sockaddr *)&local, &len) < 0) {
        strncpy(ipstr, "127,0,0,1", iplen);
        return;
    }
    b = (unsigned char *)&local.sin_addr.s_addr;
    snprintf(ipstr, iplen, "%u,%u,%u,%u", b[0], b[1], b[2], b[3]);
}

static int bind_listen_socket(void)
{
    struct sockaddr_in sa = {0};
    int sd = socket(AF_INET, SOCK_STREAM, 0);

    if (sd < 0) return -1;
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = htonl(INADDR_ANY);
    sa.sin_port = htons(0);
    if (bind(sd, (struct sockaddr *)&sa, sizeof(sa)) < 0
            || listen(sd, 1) < 0) {
        close(sd);
        return -1;
    }
    return sd;
}

int create_pasv_socket(int *out_fd, char *ipstr, int iplen,
                       int *out_port, int ctrl_fd)
{
    struct sockaddr_in bound;
    socklen_t len = sizeof(bound);
    int sd = bind_listen_socket();

    if (sd < 0) return -1;
    if (getsockname(sd, (struct sockaddr *)&bound, &len) < 0) {
        close(sd);
        return -1;
    }
    get_local_ip(ctrl_fd, ipstr, iplen);
    *out_fd = sd;
    *out_port = ntohs(bound.sin_port);
    return 0;
}

static int open_data_conn(int idx)
{
    struct sockaddr_in peer;
    socklen_t len = sizeof(peer);
    int fd;

    if (clients[idx].port_mode) {
        fd = socket(AF_INET, SOCK_STREAM, 0);
        if (fd < 0) return -1;
        if (connect(fd, (struct sockaddr *)&clients[idx].port_addr,
                sizeof(clients[idx].port_addr)) < 0) {
            close(fd);
            return -1;
        }
        return fd;
    }
    fd = accept(clients[idx].pasv_listen_fd, (struct sockaddr *)&peer, &len);
    close(clients[idx].pasv_listen_fd);
    clients[idx].pasv_listen_fd = -1;
    return fd;
}

static void send_listing(int data_fd, const char *cwd)
{
    char cmd[1100];
    char line[1024];
    FILE *ls;

    snprintf(cmd, sizeof(cmd), "ls -la %s", cwd);
    ls = popen(cmd, "r");
    if (!ls) return;
    while (fgets(line, sizeof(line), ls))
        write(data_fd, line, strlen(line));
    pclose(ls);
}

static void child_list(int idx)
{
    int data_fd = open_data_conn(idx);

    if (data_fd < 0) {
        send_reply(clients[idx].fd, "425 Can't open data connection.\r\n");
        _exit(1);
    }
    send_listing(data_fd, clients[idx].cwd);
    close(data_fd);
    send_reply(clients[idx].fd, "226 Directory send OK.\r\n");
    _exit(0);
}

void start_list_transfer(int idx)
{
    int ctrl_fd = clients[idx].fd;
    pid_t pid;

    if (clients[idx].port_mode == 0 && clients[idx].pasv_listen_fd == -1) {
        send_reply(ctrl_fd, "425 Use PORT or PASV first.\r\n");
        return;
    }
    send_reply(ctrl_fd, "150 Here comes the directory listing.\r\n");
    pid = fork();
    if (pid < 0) {
        send_reply(ctrl_fd, "451 Requested action aborted.\r\n");
        return;
    }
    if (pid == 0)
        child_list(idx);
}
