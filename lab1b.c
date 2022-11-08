#include <mqueue.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>

int main(int argc, char const *argv[]) {
  int const MSG_MAX_SIZE = 256;
  char const *MSG_QUEUE_NAME = "/lab1b_queue";
  char buf[MSG_MAX_SIZE];
  mqd_t msg_qe;
  struct mq_attr attr;
  int len;

  attr.mq_maxmsg = 1024;
  attr.mq_msgsize = MSG_MAX_SIZE;

  msg_qe = mq_open(MSG_QUEUE_NAME, O_RDWR | O_CREAT, &attr);

  switch (fork()) {
  case 0:

    msg_qe = mq_open(MSG_QUEUE_NAME, O_WRONLY);

    FILE *f = fopen(argv[1], "r");

    len = fread(buf, 1, MSG_MAX_SIZE, f);
    do {
      printf("%s", buf);
      int o = mq_send(msg_qe, buf, len, 1);
      printf("%i", o);
    } while (len == MSG_MAX_SIZE);

    break;
  case 1:
    // Error

    break;
  default:

    wait(NULL);

    int words = 0;
    char last = ' ';
    while ((len = mq_receive(msg_qe, buf, MSG_MAX_SIZE, 0)) > 0) {
      for (int i = 0; i < len; i++) {
        printf("%s", buf);
        if (last != ' ' && buf[i] == ' ') {
          words++;
        }
        last = buf[i];
      }
    }

    printf("Found %i words\n", words);

    break;
  }

  return 0;
}
