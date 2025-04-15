#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFSIZE 1024
#define OPSZ 4

void error_handling(const char *message);

int calculate(char operandCnt, int *operand, char operator);

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_sz;
    char opmsg[BUFSIZE];
    char operandCnt;     // 操作数个数
    int strLen, recvLen, recvCnt;
    int result;

    if(argc != 2) {
        printf("Usage: %s <port> \n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1)
        error_handling("socket() error!");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error!");

    if(listen(serv_sock, 5) == -1)
        error_handling("listen() error!");

    clnt_addr_sz = sizeof(clnt_addr);
    for (size_t i = 0; i < 5; i++)
    {
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_sz);
        if(clnt_sock == -1)
            error_handling("accept() error!");
        
        recvLen = read(clnt_sock, &operandCnt, 1);
        if(recvLen == -1)
            error_handling("read(operandCnt) error!");
        
        strLen = operandCnt * OPSZ + 1;
        recvLen = 0;
        while (strLen > recvLen)
        {
            recvCnt = read(clnt_sock, &opmsg[recvLen], BUFSIZE - recvLen);
            if(recvCnt == -1)
                error_handling("read(operand+operator) error!");
            
            recvLen += recvCnt;
        }

        int result = calculate(operandCnt, (int *)&opmsg[0], opmsg[operandCnt * OPSZ]);
        // key: write()这里要明确char*
        strLen = write(clnt_sock, (char*)&result, 4);
        if(strLen == -1)
            error_handling("write(calculate result) error!");
        
        close(clnt_sock);
    }
    close(serv_sock);

    return 0;
}

int calculate(char operandCnt, int *operand, char operator) {
    int result = operand[0];
    switch(operator) {
        case '+':
            for(int i = 1; i < operandCnt; ++i)
                result += operand[i];
            break;
        case '-':
            for(int i = 1; i < operandCnt; ++i)
                result -= operand[i];
            break;
        case '*':
            for(int i = 1; i < operandCnt; ++i)
                result *= operand[i];
            break;
        default:
            break;
    }
    return result;
}

void error_handling(const char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}