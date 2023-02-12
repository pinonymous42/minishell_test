#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main(void)
{
    int fd1, fd2;
    char *cmd[] = {"cat", NULL};
    char *envp[] = {NULL};

    // printf("%s, %d\n", __FILE__, __LINE__);
    fd1 = open("test.txt", O_RDONLY);
    fd2 = open("output.txt", (O_WRONLY | O_CREAT), 0777);
    // printf("%s, %d\n", __FILE__, __LINE__);
    dup2(fd1, 0);//標準入力
    dup2(fd2, 1);
    // printf("%s, %d\n", __FILE__, __LINE__);
    execve("/bin/cat", cmd, envp);
    // printf("%s, %d\n", __FILE__, __LINE__);
    return (0);
}