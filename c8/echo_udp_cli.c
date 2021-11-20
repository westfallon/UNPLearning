// P244
// 使用UDP的简单回射客户端

#include "unp.h"

void dg_cli(FILE *fp, int socket_fd, const SA *serv_addr, socklen_t serv_len) {
    int n;
    char send_line[MAXLINE], recv_line[MAXLINE];

    while (Fgets(send_line, MAXLINE, fp) != NULL) {
        Sendto(socket_fd, send_line, strlen(send_line), 0, serv_addr, serv_len);

        n = Recvfrom(socket_fd, recv_line, MAXLINE, 0, NULL, NULL);

        recv_line[n] = 0;
        Fputs(recv_line, stdout);
    }
}

int main(int argc, char **argv) {
    int socket_fd;
    struct sockaddr_in serv_addr;

    if (argc != 2) {
        err_quit("usage: udpcli <IPaddress>");
    }

    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    Inet_pton(AF_INET, argv[1], &serv_addr.sin_addr);

    socket_fd = Socket(AF_INET, SOCK_DGRAM, 0);

    dg_cli(stdin, socket_fd, (SA *) &serv_addr, sizeof(serv_addr));

    return 0;
}

