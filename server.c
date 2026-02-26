#include "server.h"

int create_server_socket(int port)
{
    struct sockaddr_in sa = {0};
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    if (fd < 0) { perror("socket"); return -1; }
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = htonl(INADDR_ANY);
    sa.sin_port = htons(port);
    if (bind(fd, (struct sockaddr *)&sa, sizeof(sa)) != 0) {
        perror("bind");
        return -1;
    }
    if (listen(fd, MAX_PENDING) == -1) {
        perror("listen");
        return -1;
    }
    return fd;
}
