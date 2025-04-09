#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFSIZE 100

void error_handling(const char *msg);

int main(void)
{
    int fd;
    char msg[BUFSIZE];
    int strLen;
    
    fd = open("data.txt", O_RDONLY);
    if(fd == -1)
        error_handling("open() error!");
    printf("file descriptor: %d\n", fd);
    
    strLen = read(fd, msg, sizeof(msg));
    if(strLen == -1)
        error_handling("read() error!");
    // 预设：strLen == 0; msg is 乱码，想错了，效果很好，读取的是正确的
    printf("read %d bytes and msg is:\n%s", strLen, msg);

    close(fd);
    return 0;
}

void error_handling(const char *msg)
{
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(-1);
}