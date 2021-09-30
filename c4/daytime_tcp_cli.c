// P5
// 时间获取客户程序

#include "unp.h"

int main(int argc, char **argv) {
    int socket_fd, n;
    char recv_line[MAXLINE + 1];
    struct sockaddr_in serv_addr;

    if (argc != 2) {
        err_quit("usage: a.out <IP_address>");
    }

    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        err_sys("socket error");
    }

    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(13);
    if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0) {
        err_quit("inet_pton error for %s", argv[1]);
    }

    if (connect(socket_fd, (SA *) &serv_addr, sizeof(serv_addr)) < 0) {
        err_sys("connect error");
    }

    while ((n = read(socket_fd, recv_line, MAXLINE)) > 0) {
        recv_line[n] = 0;
        if (fputs(recv_line, stdout) == EOF) {
            err_sys("fputs error");
        }
    }
    if (n < 0) {
        err_sys("read error");
    }
    return 0;
}

