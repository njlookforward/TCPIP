#include <stdio.h>
// fputs fputc printf
#include <stdlib.h>
// exit atoi
#include <string.h>
// memset strlen
#include <unistd.h>
// read write
#include <arpa/inet.h>
// sockaddr_in htonl htons inet_addr
#include <sys/socket.h>
// socket bind listen accept connect

void error_handling(const char *msg);

// check argc -> socket -> bind -> listen -> accept -> read and write ->close
int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr;   // Bug: 在C语言中结构体对象的类型必须加上struct
    struct sockaddr_in clnt_addr;
    socklen_t clnt_sz;
    char msg[] = "hello tcp/ip world."; // 20
    int recvLen;

    if(argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(-1);
    }
    
    serv_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(serv_sock == -1)
        error_handling("socker() error");
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error");
    
    if(listen(serv_sock, 5) == -1)
        error_handling("listen() error");
    
    clnt_sz = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (struct sockaddr*) &clnt_addr, &clnt_sz);
    if(clnt_sock == -1)
        error_handling("accept() error");

    // recvLen = read(clnt_sock, msg, BUFSIZE);
    // if(recvLen == -1)
    //     error_handling("read() error");
    // 试验sizeof()比strlen()大1
    // write(clnt_sock, msg, sizeof(msg));
    write(clnt_sock, msg, strlen(msg));
    close(clnt_sock);
    close(serv_sock);
    return 0;
}

void error_handling(const char *msg)
{
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(-1);
}