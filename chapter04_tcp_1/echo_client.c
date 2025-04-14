#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFSIZE 1024

void error_handling(const char *message);

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in serv_addr;
    char message[BUFSIZE];
    int strLen;

    if(argc != 3) {
        printf("Usage: %s <ip> <port> \n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1)
        error_handling("socket() error!");
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect() error!");
    
    while (1)
    {
        fputs("Please input what you want to say or q/Q to quit: ", stdout);
        fgets(message, BUFSIZE, stdin);
        if(!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
            break;

        strLen = write(sock, message, strlen(message));
        if(strLen == -1)
            error_handling("write() error!");
        strLen = read(sock, message, BUFSIZE-1);
        if(strLen == -1)
            error_handling("read() error!");
        message[strLen] = '\0';
        printf("Message from server: %s", message);    
        // 在客户端中是一写一读的形式，因此如果因为连接方式是基于TCP的，因为不存在数据边界
        // 有可能收到的是比较混乱的，需要订协议
    }
    close(sock);

    return 0;
}

void error_handling(const char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}