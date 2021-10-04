// P133
// 使用select的回射客户端程序

#include "unp.h"

/*
 * !!错误版本!!
 * 存在问题
 * 1、readline()和fgets()有缓冲区，而select只是从read的角度指出是否有数据可读，
 *    而不是从fgets()之类的调用的角度考虑
 * 2、考虑批量输入的情况，在标准输入出现EOF(即客户端输入结束)后，会调用return结束函数，
 *    但此时可能有应答未接收到，从而出现错误
 * */
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

/*
 * !!正确版本!!
 * 1、使用read来替代readline()和fgets()
 * 2、使用shutdown()实现关闭TCP连接的一般，表示客户端输入完成
 * 3、标准输入完成(stdin_eof == 0)并且socket数据接收完成时再退出函数，
 *    防止退出后有数据未接收到
 * */
void new_ser_cli(FILE *fp, int socket_fd) {
    int max_fdp1, stdin_eof;
    fd_set r_set;
    char buf[MAXLINE];
    int n;

    stdin_eof = 0;
    FD_ZERO(&r_set);
    for ( ; ; ) {
        if (stdin_eof == 0) {
            FD_SET(fileno(fp), &r_set);
        }
        FD_SET(socket_fd, &r_set);
        max_fdp1 = max(fileno(fp), socket_fd);
        Select(max_fdp1, &r_set, NULL, NULL, NULL);

        if (FD_ISSET(socket_fd, &r_set)) {
            if ((n = Read(socket_fd, buf, MAXLINE)) == 0) {
                if (stdin_eof == 1) {
                    return;
                }
                err_quit("str_cli: server terminated prematurely");
            }
            Write(fileno(stdout), buf, n);
        }

        if (FD_ISSET(fileno(fp), &r_set)) {
            if ((n = Read(fileno(fp), buf, MAXLINE)) == 0) {
                stdin_eof = 1;
                Shutdown(socket_fd, SHUT_WR);
                FD_CLR(fileno(fp), &r_set);
                continue;
            }
            Writen(socket_fd, buf, n);
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


