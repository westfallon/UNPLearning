// P100
// 简单的回射客户端程序

#include "unp.h"

// 客户处理循环
void str_cli(FILE *fp, int socket_fd) {
    char send_line[MAXLINE], recv_line[MAXLINE];
    while (Fgets(send_line, MAXLINE, fp) != NULL) {
        Writen(socket_fd, send_line, strlen(send_line));
        if (Readline(socket_fd, recv_line, MAXLINE) == 0) {
            err_quit("str_cli: server terminated prematurely");
        }
        Fputs(recv_line, stdout);
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


