#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>

void error_handling(const char *message);

int main(int argc, char *argv[])
{
    struct hostent *host;
    // struct in_addr hostAddr;
    struct sockaddr_in hostAddr;

    if(argc != 2) {
        printf("Usage: %s <ip> \n", argv[0]);
        exit(1);
    }

    //// @attention 主机地址一直都是用网络字节序进行处理
    //// @bug 直接使用struct in_addr是不行的
    // hostAddr.s_addr = inet_addr(argv[1]);   
    memset(&hostAddr, 0, sizeof(hostAddr));
    hostAddr.sin_addr.s_addr = inet_addr(argv[1]);

    host = gethostbyaddr((char*)&hostAddr.sin_addr, 4, AF_INET);
    if(host == NULL)
        error_handling("gethostbyaddr() error!");
    
    printf("official domain name: %s \n", host->h_name);
    for(int i = 0; host->h_aliases[i]; ++i)
        printf("domain alias %d: %s \n", i+1, host->h_aliases[i]);
    printf("ip type: %s \n", host->h_addrtype == AF_INET? "AF_INET":"AF_INET6");
    printf("ip length: %d \n", host->h_length);
    for(int i = 0; host->h_addr_list[i]; ++i)
        printf("ip %d: %s \n", i+1, inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));

    return 0;
}

void error_handling(const char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}