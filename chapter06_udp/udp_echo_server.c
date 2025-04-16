#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

//// @attention 不要吝啬，很多C程序问题就是因为数组越界
#define BUFSIZE 100

void error_handling(const char *message);

int main(int argc, char *argv[])
{
/** @brief 基于UDP的服务端/客户端，都只需要一个udp套接字即可，可以将udp套接字想象成一个邮筒，
 *  一台主机可以只通过一个udp套接字就与多台主机进行连接，中间没有建立连接，断开连接的过程
 *  因此一般情况下速度会更快，但是对于server端udp套接字需要主动分配ip和端口号，这样client端
 * 才能明确目的端的地址，而client端可以通过在第一次sendto时自动分配ip和端口号
 */ 
    int sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_sz;
    char message[BUFSIZE];
    int strLen, i;

    if(argc != 2) {
        printf("Usage: &s <port> \n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(sock == -1)
        error_handling("socket() error!");
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    if(bind(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error!");
    
    i = 0;
    while (1)
    {
        clnt_addr_sz = sizeof(clnt_addr);
        strLen = recvfrom(sock, message, BUFSIZE, 0,
                (struct sockaddr*)&clnt_addr, &clnt_addr_sz);
        if(strLen == -1)
            error_handling("recvfrom() error!");
        else
            printf("receive message %d times.\n", ++i);
        
        // BUG: 没有结束
        message[strLen] = '\0'; // 对于recvfrom()函数，需要自己手动添加'\0'结束符
        if(!strcmp(message, "end\n")) break;
        
        strLen = sendto(sock, message, strLen, 0,
            (struct sockaddr*)&clnt_addr, sizeof(clnt_addr));
        if(strLen == -1)
            error_handling("sendto() error!");
    }
    
    close(sock);
    return 0;
}

void error_handling(const char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}