#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void mask_sigchld (void) {
  sigset_t sigset;
  sigemptyset(&sigset);
  sigaddset(&sigset, SIGCHLD);
  pthread_sigmask(SIG_BLOCK, &sigset, NULL);
}

static void unmask_sigchld (void) {
  sigset_t sigset;
  sigemptyset(&sigset);
  sigaddset(&sigset, SIGCHLD);
  pthread_sigmask(SIG_UNBLOCK, &sigset, NULL);
}
 
int main(int argc, char *rgv[]) {
  pid_t pid;
  char *argv[] = {"test.txt", NULL};
  char *environ[] = {NULL};
 
  /* 複数子プロセスの終了を待つ場合など、
     fork() 処理中は SIGCHLD をブロックする*/
  mask_sigchld();
 
  pid = fork();
  if (pid > 0)
  {
    /* 親プロセス */
    write(1, "parent process\n", 15);
    wait(NULL);
    printf("%s, %d\n", __FILE__, __LINE__);
  }
  else if (pid == 0)
  {
    /* 子プロセス */
    write(1, "child process\n", 14);
	// argv[1] = rgv[1];
    /* 意図しない fd の複製によるバグを防ぐ */
    /* 0=stdin, 1=stdout, 2=stderr */
    /* 1024= ulimit -n の結果 */
    int i;
    for(i = 3; i < 1024; i++) {
      close(i);
    }
    // execve("/sbin/echo", argv, environ);//echoの絶対PATHを指定している。
    execve("/bin/echo", argv, environ);//echoの絶対PATHを指定している。
    printf("%s, %d\n", __FILE__, __LINE__);
    // perror("execve");                    /* ここに到達した場合はエラー */
    // _exit(0);
  } else {
    /* エラー */
    perror("fork");
  }
  unmask_sigchld();
  return 0;
}