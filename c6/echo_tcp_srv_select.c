// P140
// 使用单进程和select实现的回射服务器

#include "unp.h"

int main() {
    int i, max_i, max_fd, listen_fd, conn_fd, socket_fd;
    int n_ready, client[FD_SETSIZE];
    ssize_t n;
    fd_set r_set, all_set;
    char buf[MAXLINE];
    socklen_t cli_len;
    struct sockaddr_in cli_addr, serv_addr;

    listen_fd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(SERV_PORT);

    Bind(listen_fd, (SA *) &serv_addr, sizeof(serv_addr));

    Listen(listen_fd, LISTENQ);

    max_fd = listen_fd;
    max_i = -1;
    for (i = 0; i < FD_SETSIZE; i++) {
        client[i] = -1;
    }
    FD_ZERO(&all_set);
    FD_SET(listen_fd, &all_set);

    for ( ; ; ) {
        r_set = all_set;
        n_ready = Select(max_fd + 1, &r_set, NULL, NULL, NULL);

        if (FD_ISSET(listen_fd, &r_set)) {
            cli_len = sizeof(cli_addr);
            conn_fd = Accept(listen_fd, (SA *) &cli_addr, &cli_len);

            for (i = 0; i < FD_SETSIZE; i++) {
                if (client[i] < 0) {
                    client[i] = conn_fd;
                    break;
                }
            }
            if (i == FD_SETSIZE) {
                err_quit("too many clients");
                break;
            }

            FD_SET(conn_fd, &all_set);
            if (conn_fd > max_fd) {
                max_fd = conn_fd;
            }
            if (i > max_i) {
                max_i = i;
            }

            if (--n_ready <= 0) {
                continue;
            }
        }

        for (i = 0; i <= max_i; i++) {
            if ((socket_fd = client[i]) < 0) {
                continue;
            }
            if (FD_ISSET(socket_fd, &r_set)) {
                if ((n = Read(socket_fd, buf, MAXLINE)) == 0) {
                    Close(socket_fd);
                    FD_CLR(socket_fd, &all_set);
                    client[i] = -1;
                } else {
                    Writen(socket_fd, buf, n);
                }
                if (--n_ready <= 0) {
                    break;
                }
            }
        }
    }
}

