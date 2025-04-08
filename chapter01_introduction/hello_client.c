#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFSIZE 1024

void error_handling(const char *msg);

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in serv_addr;
    char msg[BUFSIZE];
    int strLen;

    if(argc != 3) {
        printf("Usage: %s [ip] [port]\n", argv[0]);
        exit(-1);
    }

    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(sock == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect() error");
    
    strLen = read(sock, msg, BUFSIZE - 1);
    msg[strLen] = '\0';
    printf("Message from server: %s | receive %d words\n", msg, strLen);

    close(sock);
    return 0;
}

void error_handling(const char *msg)
{
    fputs(msg, stderr);
    fputc('\n', stderr);
    return;
}