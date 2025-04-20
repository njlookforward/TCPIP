#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <arpa/inet.h>

void error_handling(const char *message);

int main(int argc, char *argv[])
{
    struct hostent *host;

    if(argc != 2) {
        printf("Usage: %s <domain name> \n", argv[0]);
        exit(1);
    }

    host = gethostbyname(argv[1]);
    if(host == NULL)
        error_handling("gethostbyname() error!");
    
    printf("official domain name: %s \n", host->h_name);
    for(int i = 0; host->h_aliases[i]; ++i)
        printf("domain alias %d: %s \n", i + 1, host->h_aliases[i]);
    printf("host address type: %s \n", host->h_addrtype == AF_INET? "AF_INET" : "AF_INET6");
    printf("host address lenght: %d \n", host->h_length);
    for(int i = 0; host->h_addr_list[i]; ++i)
        printf("ip %d: %s \n", i+1, inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));

    return 0;
}

void error_handling(const char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}