// P98
// 简单的回射服务器

#include "unp.h"

void sig_child(int signo) {
    pid_t pid;
    int stat;

    pid = wait(&stat);
    printf("child %d terminated\n", pid);
    return;
}

// 在socket上回射数据
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
    int listen_fd, conn_fd;
    pid_t child_pid;
    socklen_t cli_len;
    struct sockaddr_in cli_addr, serv_addr;

    listen_fd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(SERV_PORT);

    Bind(listen_fd, (SA *) &serv_addr, sizeof(serv_addr));

    Listen(listen_fd, LISTENQ);

    Signal(SIGCHLD, sig_child);

    for ( ; ; ) {
        cli_len = sizeof(cli_addr);
        if ((conn_fd = accept(listen_fd, (SA *) &cli_addr, &cli_len)) < 0) {
            if (errno == EINTR) {
                continue;
            } else {
                err_sys("accept error");
            }
        }
        if ((child_pid = Fork()) == 0) { // child process
            Close(listen_fd);
            str_echo(conn_fd);
            exit(0);
        }
        Close(conn_fd);
    }
}


