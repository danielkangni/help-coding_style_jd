#include "server.h"

static void cmd_user(int i, const char *arg)
{
    if (strcasecmp(arg, "anonymous") == 0) {
        clients[i].awaiting_pass = 1;
        strncpy(clients[i].user, arg, sizeof(clients[i].user) - 1);
        send_reply(clients[i].fd, "331 Please specify the password.\r\n");
    } else {
        send_reply(clients[i].fd, "530 Only Anonymous allowed.\r\n");
    }
}

static void cmd_pass(int i, const char *arg)
{
    if (!clients[i].awaiting_pass) {
        send_reply(clients[i].fd, "503 Login with USER first.\r\n");
        return;
    }
    if (strlen(arg) == 0) {
        clients[i].logged_in = 1;
        clients[i].awaiting_pass = 0;
        send_reply(clients[i].fd, "230 Login successful.\r\n");
    } else {
        send_reply(clients[i].fd, "530 Login incorrect.\r\n");
    }
}

static void cmd_quit(int i, struct pollfd *fds, int *nfds)
{
    send_reply(clients[i].fd, "221 Goodbye.\r\n");
    cleanup_client(i, fds, nfds);
}

static void cmd_pwd(int i)
{
    char reply[1200];

    snprintf(reply, sizeof(reply),
        "257 \"%s\" is the current directory.\r\n", clients[i].cwd);
    send_reply(clients[i].fd, reply);
}

static void cmd_cwd(int i, const char *arg)
{
    char newpath[2048];

    if (!clients[i].logged_in) {
        send_reply(clients[i].fd, "530 Not logged in.\r\n");
        return;
    }
    if (arg[0] == '/')
        snprintf(newpath, sizeof(newpath), "%s%s", home_path, arg);
    else
        snprintf(newpath, sizeof(newpath), "%s/%s", clients[i].cwd, arg);
    if (chdir(newpath) == 0) {
        strncpy(clients[i].cwd, newpath, sizeof(clients[i].cwd) - 1);
        send_reply(clients[i].fd, "250 Directory successfully changed.\r\n");
    } else {
        send_reply(clients[i].fd, "550 Failed to change directory.\r\n");
    }
}

static void cmd_pasv(int i)
{
    int pasv_fd, pasv_port;
    char ipfmt[64];
    char reply[200];

    if (create_pasv_socket(&pasv_fd, ipfmt, sizeof(ipfmt),
            &pasv_port, clients[i].fd) != 0) {
        send_reply(clients[i].fd, "425 Can't open passive connection.\r\n");
        return;
    }
    clients[i].pasv_listen_fd = pasv_fd;
    clients[i].port_mode = 0;
    snprintf(reply, sizeof(reply), "227 Entering Passive Mode (%s,%u,%u).\r\n",
        ipfmt, pasv_port / 256, pasv_port % 256);
    send_reply(clients[i].fd, reply);
}

static void cmd_port(int i, const char *arg)
{
    int h1, h2, h3, h4, p1, p2;
    char ipstr[32];

    if (sscanf(arg, "%d,%d,%d,%d,%d,%d", &h1, &h2, &h3, &h4, &p1, &p2) != 6) {
        send_reply(clients[i].fd, "501 Syntax error in parameters.\r\n");
        return;
    }
    snprintf(ipstr, sizeof(ipstr), "%d.%d.%d.%d", h1, h2, h3, h4);
    clients[i].port_mode = 1;
    clients[i].pasv_listen_fd = -1;
    memset(&clients[i].port_addr, 0, sizeof(clients[i].port_addr));
    clients[i].port_addr.sin_family = AF_INET;
    clients[i].port_addr.sin_addr.s_addr = inet_addr(ipstr);
    clients[i].port_addr.sin_port = htons(p1 * 256 + p2);
    send_reply(clients[i].fd, "200 PORT command successful.\r\n");
}

static void cmd_list(int i)
{
    if (!clients[i].logged_in) {
        send_reply(clients[i].fd, "530 Not logged in.\r\n");
        return;
    }
    start_list_transfer(i);
}

void dispatch_command(int i, const char *cmd, const char *arg,
                      struct pollfd *fds, int *nfds)
{
    if (strcmp(cmd, "USER") == 0) { cmd_user(i, arg); return; }
    if (strcmp(cmd, "PASS") == 0) { cmd_pass(i, arg); return; }
    if (strcmp(cmd, "QUIT") == 0) { cmd_quit(i, fds, nfds); return; }
    if (strcmp(cmd, "PWD") == 0)  { cmd_pwd(i); return; }
    if (strcmp(cmd, "CWD") == 0)  { cmd_cwd(i, arg); return; }
    if (strcmp(cmd, "PASV") == 0) { cmd_pasv(i); return; }
    if (strcmp(cmd, "PORT") == 0) { cmd_port(i, arg); return; }
    if (strcmp(cmd, "LIST") == 0) { cmd_list(i); return; }
    send_reply(clients[i].fd, "502 Command not implemented.\r\n");
}
