#include <stdio.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>        /* TCP_NODELAY, TCP_CORK */
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/un.h>

#include <errno.h>

#include <string.h>

int
main(int argc, char *const *argv)
{
    int fd;
    struct sockaddr_in addr;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        printf("create socket fail, errno: %d", errno);
        return -1;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(8060);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        printf("bind fail, errno: %d", errno);
        return -1;
    }

    if(listen(fd, 20) == -1) {
        printf("listen fail, errno: %d", errno);
        return -1;
    }

    struct sockaddr_in cli_addr;
    socklen_t addr_len = sizeof(cli_addr);
    int conn = accept(fd, (struct sockaddr*)&cli_addr, &addr_len);
    if (conn < 0) {
        printf("accept fail, errno: %d", errno);
        return -1;
    }
    printf("client connect\n");
    char buff[1024];
    memset(buff, 0, sizeof(buff));
    sprintf(buff, "aaaaa");
    int len = send(conn, buff, strlen(buff), 0);
    while (1) {
        memset(buff, 0, sizeof(buff));
        len = recv(conn, buff, sizeof(buff), 0);
        if (len <= 0) {
            printf("client disconnect, errno: %d", errno);
            break;
        }
        printf("got resp: %s", buff);
    }
    return 0;
}
