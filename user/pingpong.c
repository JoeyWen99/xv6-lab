#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char* argv[]) 
{
  // 使用创建管道
  int father_pipe[2];
  int child_pipe[2];
  pipe(father_pipe);
  pipe(child_pipe);

  // 使用fork创建子进程，
  // 使用 read 从管道，使用 write 写入管道
  // 使用 getpid 查找调用进程的进程ID。
  if (fork() == 0) {
    // 子进程
    // fork创建完子进程后, 在父进程中打开的file descriptor会被复制保持打开状态到子进程中
    // the child should print "<pid>: received ping"
    char buf = read(father_pipe[0], &buf, 1);
    printf("%d: received ping\n", getpid());
    write(child_pipe[1], &buf, 1);
    exit(0);
  }
  else {
    // 父进程
    // the parent should read the byte from the child, print "<pid>: received pong"
    char buf1 = 'a'; 
    write(father_pipe[1], &buf1, 1);
    buf1 = read(child_pipe[0], &buf1, 1);
    printf("%d: received pong\n", getpid());
    exit(0);
  }
}
