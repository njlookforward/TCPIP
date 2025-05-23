#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(const char *msg);

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;
    char msg[] = "hello tcp/ip world!";
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_sz;

    if(argc != 2) {
        printf("Usage &s <port>\n", argv[0]);
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
    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_sz);
    if(write(clnt_sock, msg, sizeof(msg)) == -1)
        error_handling("write() error!");
    
    close(clnt_sock);
    close(serv_sock);
    return 0;
}

void error_handling(const char *msg)
{
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(1);
}