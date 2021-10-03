// P133
// 使用select的回射客户端程序

#include "unp.h"

void str_cli(FILE *fp, int socket_fd) {
    int max_fdp1;
    fd_set r_set;
    char send_line[MAXLINE], recv_line[MAXLINE];

    FD_ZERO(&r_set);
    for ( ; ; ) {
        FD_SET(fileno(fp), &r_set);
        FD_SET(socket_fd, &r_set);
        max_fdp1 = max(fileno(fp), socket_fd) + 1;
        Select(max_fdp1, &r_set, NULL, NULL, NULL);

        if (FD_ISSET(socket_fd, &r_set)) {
            if (Readline(socket_fd, recv_line, MAXLINE) == 0) {
                err_quit("str_cli: server terminated prematurely");
            }
            Fputs(recv_line, stdout);
        }

        if (FD_ISSET(fileno(fp), &r_set)) {
            if (Fgets(send_line, MAXLINE, fp) == NULL) {
                return;
            }
            Writen(socket_fd, send_line, strlen(send_line));
        }
    }
}

int main(int argc, char **argv) {
    int socket_fd;
    struct sockaddr_in serv_addr;

    if (argc != 2) {
        err_quit("usage: echocli <IPaddress>");
    }

    socket_fd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    Inet_pton(AF_INET, argv[1], &serv_addr.sin_addr);

    Connect(socket_fd, (SA *) &serv_addr, sizeof(serv_addr));

    str_cli(stdin, socket_fd);
    exit(0);
}


