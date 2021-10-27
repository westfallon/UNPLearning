// P146
// 使用poll的回射服务器

#include "unp.h"
#include <limits.h>

int main() {
    int i, max_i, listen_fd, conn_fd, socket_fd;
    int n_ready;
    ssize_t n;
    char buf[MAXLINE];
    socklen_t cli_len;
    struct pollfd client[OPEN_MAX];
    struct sockaddr_in cli_addr, serv_addr;

    listen_fd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(SERV_PORT);

    Bind(listen_fd, (SA *) &serv_addr, sizeof(serv_addr));

    Listen(listen_fd, LISTENQ);

    client[0].fd = listen_fd;
    client[0].events = POLLRDNORM;
    for (i = 1; i < OPEN_MAX; i++) {
        client[i].fd = -1;
    }
    max_i = 0;

    for ( ; ; ) {
        n_ready = Poll(client, max_i + 1, INFTIM);

        if (client[0].revents & POLLRDNORM) {
            cli_len = sizeof(cli_addr);
            conn_fd = Accept(listen_fd, (SA *) &cli_addr, &cli_len);

            for (i = 1; i < OPEN_MAX; i++) {
                if (client[i].fd < 0) {
                    client[i].fd = conn_fd;
                    break;
                }
            }
            if (i == OPEN_MAX) {
                err_quit("too many clients");
                break;
            }

            client[i].events = POLLRDNORM;
            if (i > max_i) {
                max_i = i;
            }

            if (--n_ready <= 0) {
                continue;
            }
        }

        for (i = 1; i <= max_i; i++) {
            if ( (socket_fd = client[i].fd) < 0) {
                continue;
            }
            if (client[i].revents & (POLLRDNORM | POLLERR)) {
                if ( (n = read(socket_fd, buf, MAXLINE)) < 0) {
                    if (errno == ECONNRESET) {
                        Close(socket_fd);
                        client[i].fd = -1;
                    } else {
                        err_sys("read error");
                    }
                } else if (n == 0) {
                    Close(socket_fd);
                    client[i].fd = -1;
                } else {
                    printf("%s", buf);
                    Writen(socket_fd, buf, n);
                }

                if (--n_ready <= 0) {
                    break;
                }
            }
        }
    }
}


