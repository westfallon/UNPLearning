// P241
// 使用UDP的简单回射服务器

#include "unp.h"

void dg_echo(int socket_fd, SA *cli_addr, socklen_t cli_len) {
    int n;
    socklen_t len;
    char mesg[MAXLINE];

    for ( ; ; ) {
        len = cli_len;
        n = Recvfrom(socket_fd, mesg, MAXLINE, 0, cli_addr, &len);

        Sendto(socket_fd, mesg, n, 0, cli_addr, len);
    }
}

int main() {
    int socket_fd;
    struct sockaddr_in serv_addr, cli_addr;

    socket_fd = Socket(AF_INET, SOCK_DGRAM, 0);

    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(SERV_PORT);

    Bind(socket_fd, (SA *) &serv_addr, sizeof(serv_addr));

    dg_echo(socket_fd, (SA *) &cli_addr, sizeof(cli_addr));

    return 0;
}

