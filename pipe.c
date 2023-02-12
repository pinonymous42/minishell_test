#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
int main()
{
	int pipefd[2];
	int fd = open("test.txt", (O_WRONLY | O_CREAT), 0777);
	if (pipe(pipefd) < 0) {
		perror("pipe");
		exit(-1);
	}
	pid_t pid = fork();
	if (pid < 0) {
		perror("fork");
		exit(-1);
	} else if (pid == 0) {
		// 子プロセス
		close(pipefd[0]);
		char *s = "send from child";
		printf("%s\n", s);
		// dup2(pipefd[1], 1);
		write(pipefd[1], s, strlen(s));
		close(pipefd[1]);
	} else {
		// 親プロセス
		// sleep(1);
		// wait(NULL);
		close(pipefd[1]); //読み込みをクローズ
		char buf[256];
		read(pipefd[0], buf, 256);
		write(fd, buf, strlen(buf));
		close(pipefd[0]);
		// wait(NULL);
	}
}