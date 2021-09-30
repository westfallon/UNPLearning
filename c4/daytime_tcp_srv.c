// P89
// 获取时间服务器, 输出连接到服务器的客户端

#include "unp.h"
#include <time.h>

int main() {
    int listen_fd, conn_fd;
    socklen_t len;
    struct sockaddr_in serv_addr, cli_addr;
    char buff[MAXLINE];
    time_t ticks;

    listen_fd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(13);

    Bind(listen_fd, (SA *) &serv_addr, sizeof(serv_addr));

    Listen(listen_fd, LISTENQ);

    for(;;) {
        len = sizeof(cli_addr);
        conn_fd = Accept(listen_fd, (SA *) &cli_addr, &len);
        if (conn_fd < 0) {
            printf("accept error");
            break;
        }
        printf("connection from %s, port %d\n",
               Inet_ntop(AF_INET, &cli_addr.sin_addr, buff, sizeof(buff)),
               ntohs(cli_addr.sin_port));
        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        Write(conn_fd, buff, strlen(buff));

        Close(conn_fd);
    }
    return 0;
}