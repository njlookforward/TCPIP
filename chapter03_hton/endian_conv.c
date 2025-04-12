#include <stdio.h>
#include <arpa/inet.h>

#include <stdint.h>

int main(void)
{
    uint16_t host_port = 0x1234;
    uint16_t net_port;
    uint32_t host_addr = 0x12345678;
    uint32_t net_addr;

    net_port = htons(host_port);
    net_addr = htonl(host_addr);

    printf("Host ordered Port: %#x\n", host_port);
    printf("Net ordered Port: %#x\n", net_port);
    printf("Host ordered Addredd: %#lx\n", host_addr);
    printf("Net ordered Address: %#lx\n", net_addr);

    return 0;
}