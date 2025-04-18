#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFSIZE 30

void error_handling(const char *msg);

int main(int argc, char *argv[]) {
    int sd;
    struct sockaddr_in serv_addr;
    char message[BUFSIZE];
    int readLen, writeLen;
    FILE *fd = fopen("receive.dat", "wb");  // 我发现加不加'b'是无所谓的

    if(fd == NULL)
        error_handling("fopen() error!");

    if(argc != 3) {
        printf("Usage: %s <ip> <port> \n", argv[0]);
        exit(1);
    }

    sd = socket(PF_INET, SOCK_STREAM, 0);
    if(sd == -1)
        error_handling("socket() error!");
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect() error!");
    
    while (readLen = read(sd, message, BUFSIZE))
    {
        if(readLen == -1)
            error_handling("read() error!");

        writeLen = fwrite(message, 1, readLen, fd);    
    }
    puts("Received file date.\n");

    writeLen = write(sd, "Thank you!\n", strlen("Thank you!\n"));
    if(writeLen == -1)
        error_handling("write() error!");
    
    close(sd);
    fclose(fd);
    return 0;
}

void error_handling(const char *msg) {
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(1);
}