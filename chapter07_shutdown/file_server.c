#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFSIZE 30

/**
 * @brief 进行优雅地半关闭TCP连接
*/

void error_handling(const char *msg);

int main(int argc, char *argv[]) {
    int serv_sd, clnt_sd;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_sz;
    char message[BUFSIZE];
    size_t strLen, sendLen;

    FILE *fd = fopen("file_server.c", "rb");
    if(fd == NULL)
        error_handling("fopen() error!");

    if(argc != 2) {
        printf("Usage: %s <port> \n", argv[0]);
        exit(1);
    }

    serv_sd = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sd == -1)
        error_handling("socket() error!");
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error!");
    
    if(listen(serv_sd, 5) == -1)
        error_handling("listen() error!");
    
    clnt_addr_sz = sizeof(clnt_addr);
    clnt_sd = accept(serv_sd, (struct sockaddr*)&clnt_addr, &clnt_addr_sz);
    if(clnt_sd == -1)
        error_handling("accept() error!");
    while (strLen = fread(message, 1, BUFSIZE, fd))
    {
        sendLen = write(clnt_sd, message, strLen);
        if(sendLen == -1)
            error_handling("write() error!");
    }
    
    if(shutdown(clnt_sd, SHUT_WR) == -1)
        error_handling("shutdown() error!");
    
    strLen = read(clnt_sd, message, BUFSIZE);
    if(strLen == -1)
        error_handling("read() error!");
    message[strLen] = '\0';
    printf("Message from client: %s", message);
    close(clnt_sd);
    close(serv_sd);
    fclose(fd);

    return 0;
}

void error_handling(const char *msg) {
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(1);
}