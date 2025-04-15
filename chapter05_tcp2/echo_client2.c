#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFSIZE 1024

void error_handling(const char *message);

// 基于流（TCP）的网络连接，需要设计应用层协议，怎么发送，怎么接收
int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in serv_addr;
    char message[BUFSIZE];
    int strLen, recvLen, recvCnt;

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
    else 
        printf("Connecting ......\n");
    
    // 在客户端设计简单的应用层协议，这是网络程序员主要做的事情，在操作系统tcp/ip协议栈之外自定义的操作
    while (1)
    {
        fputs("Please input what you want to say or q/Q to quit: ", stdout);
        fgets(message, BUFSIZE, stdin);
        if(!strcmp(message, "q\n") || !strcmp(message, "Q\n")) {
            break;
        }
        
        strLen = write(sock, message, strlen(message));
        if(strLen == -1)
            error_handling("write() error!");
        
        recvLen = 0;
        while (recvLen < strLen)
        {
            recvCnt = read(sock, &message[recvLen], BUFSIZE-1-recvLen);
            if(recvCnt == -1)
                error_handling("read() error!");

            recvLen += recvCnt;    
        }
        message[recvLen] = '\0';
        printf("Message from server: %s", message);
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