#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdint.h>

int main(void)
{
    char *addr1 = "1.2.3.4";
    char *addr2 = "1.2.3.256";
    // char *addr1="127.212.124.78";
	// char *addr2="127.212.124.256";

    uint32_t conv_addr1 = inet_addr(addr1);
    if(conv_addr1 == INADDR_NONE)
        printf("Error occured! \n");
    else
        printf("Network ordered integer addr: %#lx \n", conv_addr1);    // 0x4030201
    
    uint32_t conv_addr2 = inet_addr(addr2);
    if(conv_addr2 == INADDR_NONE)
        printf("Error occured! \n");
    else
        printf("Network ordered integer addr: %#lx \n", conv_addr2);

    return 0;
}