// P244
// 使用UDP的简单回射客户端

#include "unp.h"

/**
 * 无法辨别接收到的信息是否为正确的服务端发送
 */
void dg_cli(FILE *fp, int socket_fd, const SA *serv_addr, socklen_t serv_len) {
    int n;
    char send_line[MAXLINE], recv_line[MAXLINE + 1];

    while (Fgets(send_line, MAXLINE, fp) != NULL) {
        Sendto(socket_fd, send_line, strlen(send_line), 0, serv_addr, serv_len);

        n = Recvfrom(socket_fd, recv_line, MAXLINE, 0, NULL, NULL);

        recv_line[n] = 0;
        Fputs(recv_line, stdout);
    }
}

/**
 * 比较接受信息的地址是否为期待的地址
 */
void new_dg_cli(FILE* fp, int socket_fd, const SA *serv_addr, socklen_t serv_len) {
    int n;
    char send_line[MAXLINE], recv_line[MAXLINE + 1];
    socklen_t len;
    struct sockaddr *reply_addr;

    reply_addr = Malloc(serv_len);

    while (Fgets(send_line, MAXLINE, fp) != NULL) {
        Sendto(socket_fd, send_line, strlen(send_line), 0, serv_addr, serv_len);

        len = serv_len;
        n = Recvfrom(socket_fd, recv_line, MAXLINE, 0, reply_addr, &len);
        if (len != serv_len || strcmp(Sock_ntop(serv_addr, serv_len), Sock_ntop(reply_addr, len))!= 0) {
            printf("reply from %s (ignored)\n", Sock_ntop(reply_addr, len));
            continue;
        }

        recv_line[n] = 0;
        printf("receive from %s : %s\n", Sock_ntop(reply_addr, len), recv_line);
    }
}

/**
 * 使用connect建立UDP连接，当目的地址不可用时会返回错误
 */
void dg_cli_connect(FILE *fp, int socket_fd, const SA *serv_addr, socklen_t serv_len) {
    int n;
    char send_line[MAXLINE], recv_line[MAXLINE + 1];

    Connect(socket_fd, (SA *) serv_addr, serv_len);

    while (Fgets(send_line, MAXLINE, fp) != NULL) {
        Write(socket_fd, send_line, strlen(send_line));

        n = Read(socket_fd, recv_line, MAXLINE);

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

//    dg_cli(stdin, socket_fd, (SA *) &serv_addr, sizeof(serv_addr));
//    new_dg_cli(stdin, socket_fd, (SA *) &serv_addr, sizeof(serv_addr));
    dg_cli_connect(stdin, socket_fd, (SA *) &serv_addr, sizeof(serv_addr));

    return 0;
}

