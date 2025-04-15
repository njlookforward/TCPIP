#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFSIZE 1024
#define RESULT_SZ 4
#define OPSZ 4

void error_handling(const char *message);

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in serv_addr;
    char opmsg[BUFSIZE];
    int operandCnt;  // 操作数数量
    // int operand;    // 记录输入的操作数
    // char operator;  // 操作符
    int result;
    int strLen;

    if(argc != 3) {
        printf("Usage: %s <IP> <port> \n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1)
        error_handling("socket() error!");
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect() error!");
    else 
        printf("Connecting ......\n");
    
    fputs("operand count: ", stdout);
    scanf("%d", &operandCnt);
    opmsg[0] = (char)operandCnt;
    for (size_t i = 0; i < operandCnt; i++)
    {
        printf("operand %d: ", i + 1);
        scanf("%d", (int *)&opmsg[i * OPSZ + 1]);
    }
    fgetc(stdin);
    fputs("operator: ", stdout);
    scanf("%c", &opmsg[operandCnt * OPSZ + 1]);

    strLen = write(sock, opmsg, operandCnt * OPSZ + 2);
    if(strLen == -1)
        error_handling("write(opmsg) error!");
    strLen = read(sock, &result, RESULT_SZ);
    if(strLen == -1)
        error_handling("read(result) error!");
    printf("operation result: %d\n", result);

    close(sock);
    return 0;
}

void error_handling(const char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}