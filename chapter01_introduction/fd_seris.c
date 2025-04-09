#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(const char *msg);

int main(void)
{
    int fd1, fd2, fd3;
    fd1 = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    fd2 = open("test.dat", O_CREAT | O_WRONLY | O_APPEND);
    fd3 = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

    printf("file descriptor fd1: %d\n", fd1);
    printf("file descriptor fd2: %d\n", fd2);
    printf("file descriptor fd3: %d\n", fd3);

    close(fd3);
    close(fd2);
    close(fd1);

    return 0;
}

void error_handling(const char *msg)
{
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(-1);
}