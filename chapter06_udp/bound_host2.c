#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFSIZE 50

/**
 * @details 验证基于udp传输的数据是有边界的，连接的双方对应的recvfrom and sendto次数必须相同
*/

void error_handling(const char *msg);

int main(int argc, char *argv[]) {
    int sock;
    struct sockaddr_in serv_addr;
    int strLen;
    char *message[] = {
        "Hi!",
        "I'm another UDP host!",
        "Nice to meet you!"
    };

    if(argc != 3) {
        printf("Usage: %s <ip> <port> \n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(sock == -1)
        error_handling("socket() error!");
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    for(int i = 0; i < 3; ++i) {
        strLen = sendto(sock, message[i], strlen(message[i]), 0,
            (struct sockaddr*)&serv_addr, sizeof(serv_addr));
        if(strLen == -1)
            error_handling("sendto() error!");
    }

    close(sock);
    return 0;
}

void error_handling(const char *msg) {
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(1);
}