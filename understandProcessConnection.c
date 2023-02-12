#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

char *cmd1[] = {"ls", NULL};
char *cmd2[] = {"cat", NULL};
// char ***cmds = {cmd1, cmd2, NULL};
char **envp = {NULL};

int main(void)
{
    int pfd[2];
    pid_t pid;

    if (pipe(pfd) == -1)
        perror("pipe");
    if ((pid = fork()) == -1)
        perror("fork");
    else if (pid == 0)
    {
        if (close(pfd[0]) == -1)
            perror("close");
        if (dup2(pfd[1], 1) == -1)
            perror("dup2");
        if (close(pfd[1]) == -1)
            perror("close");
        execve("/bin/ls", cmd1, envp);
    }
    else
    {
        wait(NULL);
        if (close(pfd[1]) == -1)
            perror("close");
        if (dup2(pfd[0], 0) == -1)
            perror("dup2");
        if (close(pfd[0]) == -1)
            perror("close");
        execve("/usr/bin/wc", cmd2, envp);
    }
    return (0);
}