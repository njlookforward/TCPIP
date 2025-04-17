#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

//// @details 使用已连接udp套接字在进行持续数据传输时，能够提高效率，而且在套接字中注册目的地址信息后
// 可以使用read and write函数进行I/O操作
#define BUFSIZE 100

void error_handling(const char *message);

//// @attention 9190明明已经断开了，怎么client还可以连接？？？
// 我目前的想法是：不要搞混tcp和udp，udp类似于邮箱，已经在信中写好了对方的地址，投出去就好了，即使对方地址不存在也无所谓
// 即使我没有打开服务器端，客户端仍然可以发送信息，只不过最后因为recvfrom而陷入阻塞

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in serv_addr /*from_addr*/;    // 在本程序中两者应该是相同的
    // socklen_t from_addr_sz;
    char message[BUFSIZE];
    int strLen;

    if(argc != 3) {
        printf("Usage: %s <IP> <port> \n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(sock == -1)
        error_handling("socket() error!");
    
    // 在建立连接之前，需要指定服务器端地址信息
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect() error!");

    while (1)
    {
        fputs("insert Message or q/Q to quit: ", stdout);
        fgets(message, BUFSIZE, stdin);
        if(!strcmp(message, "q\n") || !strcmp(message, "Q\n")) break;
        // strLen = sendto(sock, message, strlen(message), 0,
        //     (struct sockaddr*)&serv_addr, sizeof(serv_addr));
        strLen = write(sock, message, strlen(message));
        if(strLen == -1)
            error_handling("sendto() error!");
        
        // from_addr_sz = sizeof(from_addr);
        // strLen = recvfrom(sock, message, BUFSIZE-1, 0,
        //     (struct sockaddr*)&from_addr, &from_addr_sz);
        strLen = read(sock, message, BUFSIZE-1);
        if(strLen == -1)
            error_handling("recvfrom() error!");
        message[strLen] = '\0';
        printf("Message from server: %s", message);
        // BUG：这样逻辑就通顺了
        if(!strcmp(message, "end\n"))
            break;
    }

    close(sock);
    return 0;
}

void error_handling(const char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}