#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void error_handling(const char *msg);

int main(void)
{
    char *addr = "127.232.124.79";
    struct sockaddr_in conv_addr;

    if(!inet_aton(addr, &conv_addr.sin_addr))
        error_handling("inet_aton() error!");
    else 
        printf("Network ordered integer addr: %#x \n", conv_addr.sin_addr.s_addr);

    return 0;
}

void error_handling(const char *msg)
{
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(1);
}