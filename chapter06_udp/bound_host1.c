#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFSIZE 50

void error_handling(const char *msg);

int main(int argc, char *argv[]) {
    int sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_sz;
    char message[BUFSIZE];
    int strLen, i;

    if(argc != 2) {
        printf("Usage: %s <port> \n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(sock == -1)
        error_handling("socket() error!");
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error!");
    
    strLen = 1;
    i = 3;
    clnt_addr_sz = sizeof(clnt_addr);
    while (i--)
    {
        sleep(5);
        strLen = recvfrom(sock, message, BUFSIZE, 0,
            (struct sockaddr*)&clnt_addr, &clnt_addr_sz);
        if(strLen == -1)
            error_handling("recvfrom() error!");
        message[strLen] = '\0';
        printf("receive %d times: %s \n", 3 - i, message);
    }
    
    close(sock);
    return 0;
}

void error_handling(const char *msg) {
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(1);
}