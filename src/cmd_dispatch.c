/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** Command dispatch
*/

#include "../include/server.h"

static int dispatch_auth(int i, const char *cmd,
    const char *arg)
{
    if (strcmp(cmd, "USER") == 0) {
        cmd_user(i, arg);
        return (1);
    }
    if (strcmp(cmd, "PASS") == 0) {
        cmd_pass(i, arg);
        return (1);
    }
    if (strcmp(cmd, "QUIT") == 0) {
        cmd_quit(i);
        return (1);
    }
    return (0);
}

static int dispatch_nav(int i, const char *cmd,
    const char *arg)
{
    if (strcmp(cmd, "PWD") == 0) {
        cmd_pwd(i);
        return (1);
    }
    if (strcmp(cmd, "CWD") == 0) {
        cmd_cwd(i, arg);
        return (1);
    }
    if (strcmp(cmd, "PASV") == 0) {
        cmd_pasv(i);
        return (1);
    }
    if (strcmp(cmd, "PORT") == 0) {
        cmd_port(i, arg);
        return (1);
    }
    return (0);
}

static int dispatch_extra(int i, const char *cmd)
{
    if (strcmp(cmd, "LIST") == 0) {
        cmd_list(i);
        return (1);
    }
    if (strcmp(cmd, "NOOP") == 0) {
        cmd_noop(i);
        return (1);
    }
    if (strcmp(cmd, "HELP") == 0) {
        cmd_help(i);
        return (1);
    }
    return (0);
}

void dispatch_command(int i, const char *cmd,
    const char *arg)
{
    if (cmd[0] == '\0') {
        send_reply(clients[i].fd,
            "500 Syntax error.\r\n");
        return;
    }
    if (dispatch_auth(i, cmd, arg))
        return;
    if (dispatch_nav(i, cmd, arg))
        return;
    if (dispatch_extra(i, cmd))
        return;
    send_reply(clients[i].fd,
        "500 Syntax error, command unrecognized.\r\n");
}
