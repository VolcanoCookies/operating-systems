#include <stdio.h>
#include <unistd.h>
#include <wait.h>

int main(int argc, char const *argv[]) {
  int pipefd[2];
  pipe(pipefd);

  pid_t cpid = fork();

  switch (cpid) {
  case 0:
    // Child

    close(pipefd[0]);

    dup2(pipefd[1], STDOUT_FILENO);
    execl("/bin/ls", "ls", "/", NULL);

    close(pipefd[1]);

    break;
  case -1:
    // Error
    perror("Failed to fork");
    break;
  default:
    // Parent

    close(pipefd[1]);

    wait(NULL);

    dup2(pipefd[0], STDIN_FILENO);
    execl("/bin/wc", "wc", "-l", NULL);

    close(pipefd[0]);
    break;
  }

  return 0;
}
