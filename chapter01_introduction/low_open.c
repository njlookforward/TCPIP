#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define BUFSIZE 50

void error_handling(const char *msg);

int main() 
{
    int fd;
    char msg[BUFSIZE];

    fd = open("data.txt", O_CREAT | O_RDWR | O_TRUNC);
    if(fd == -1)
        error_handling("open() error!");
    
    printf("Please input what you want to store:");
    fgets(msg, BUFSIZE, stdin);
    if(write(fd, msg, strlen(msg)) == -1)
        error_handling("write() error!");

    close(fd);
    return 0;
}

void error_handling(const char *msg)
{
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(-1);
}