// P205
// 使用select函数的TCP和UDPhis和服务器

#include "unp.h"

void sig_child(int signo) {
    pid_t pid;
    int stat;

    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0) {
        printf("child %d terminated\n", pid);
    }
//    pid = wait(&stat);
//    printf("child %d terminated\n", pid);
    return;
}

void str_echo(int socket_fd) {
    ssize_t n;
    char buf[MAXLINE];

    for (;;) {
        while ((n = read(socket_fd, buf, MAXLINE)) > 0) {
            Writen(socket_fd, buf, n);
        }
        if (n < 0 && errno == EINTR) {
            continue;
        }
        if (n < 0) {
            err_sys("str_echo: read error");
        }
        break;
    }
}

int main() {
    int listen_fd, conn_fd, udp_fd, n_ready, max_fd;
    char msg[MAXLINE];
    pid_t child_pid;
    fd_set r_set;
    ssize_t n;
    socklen_t len;
    const int on = 1;
    struct sockaddr_in cli_addr, serv_addr;

    //create listening TCP socket
    listen_fd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(SERV_PORT);

    Setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    Bind(listen_fd, (SA *) &serv_addr, sizeof(serv_addr));

    Listen(listen_fd, LISTENQ);

    // create UDP socket
    udp_fd = Socket(AF_INET, SOCK_DGRAM, 0);

    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(SERV_PORT);

    Bind(udp_fd, (SA *) &serv_addr, sizeof(serv_addr));

    Signal(SIGCHLD, sig_child);

    FD_ZERO(&r_set);
    max_fd = max(listen_fd, udp_fd) + 1;
    for ( ; ; ) {
        FD_SET(listen_fd, &r_set);
        FD_SET(udp_fd, &r_set);
        if ( (n_ready = select(max_fd, &r_set, NULL, NULL, NULL)) < 0) {
            if (errno == EINTR) {
                continue;
            } else {
                err_sys("select error");
            }
        }
        if (FD_ISSET(listen_fd, &r_set)) {
            len = sizeof(cli_addr);
            conn_fd = Accept(listen_fd, (SA *) &cli_addr, &len);

            if ( (child_pid = Fork()) == 0) { // child process
                Close(listen_fd);
                str_echo(conn_fd);
                exit(0);
            }
            Close(conn_fd);
        }
        if (FD_ISSET(udp_fd, &r_set)) {
            len = sizeof(cli_addr);
            n = Recvfrom(udp_fd, msg, MAXLINE, 0, (SA *) &cli_addr, &len);

            Sendto(udp_fd, msg, n, 0, (SA *) &cli_addr, len);
        }
    }
}

