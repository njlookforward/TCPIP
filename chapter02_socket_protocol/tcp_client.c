#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFSIZE 100

void error_handling(const char *msg);

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in sock_addr;
    char msg[BUFSIZE];
    int sumLen = 0, onceLen = 0, i = 0;

    if(argc != 3) {
        printf("Usage: %s <ip> <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1) {
        error_handling("socket() error!");
    }

    memset(&sock_addr, 0, sizeof(sock_addr));
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = inet_addr(argv[1]); // 使用这个函数更快一点，得到的size_t直接就是大端存储
    sock_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) == -1)
        error_handling("connect() error!");
    
    // 这个过程是在验证基于流TCP的网络传输，传输的数据是没有边界的
    while (onceLen = read(sock, &msg[i++], 1))
    {
        if(onceLen == -1)
            error_handling("read() error!");

        sumLen += onceLen;
    }
    
    printf("Message from server: %s\nsumLen = %d\tonceLen = %d\ti = %d\n", msg, sumLen, onceLen, i);

    // Bug: 忘记关闭套接字了
    close(sock);
    return 0;
}

void error_handling(const char *msg)
{
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(1);
}