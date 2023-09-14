#include "kernel/types.h"
#include "user/user.h"
// p = get a number from left neighbor
//   print p
//   loop :
//     n = get a number from left neighbor
//     if (p does not divide n)
//       send n to right neighbor
enum { Read, Write };
enum { False, True };

void start_process(int readfd) {
  int num;
  int my_prime = -1;
  int child_pid;
  int fd[2];
  int has_chlid = False;
  pipe(fd);

  // 读的第一个数设置为my_prime,并打印
  read(readfd, &my_prime, 4);
  printf("prime %d\n", my_prime);

  while (1)
  {
    // 从管道中读取一个数
    if (read(readfd, &num, 4) == 0)
    {
      close(readfd);
      // 管道内的数据以读完
      if (has_chlid == False)//叶子结点
      {
        exit(0);
      }
      else// 非叶子结点，创建进程处理管道内数据
      {
        child_pid = fork();

        if (child_pid == 0)
        {
          // 子进程，启动下一个进程
          close(fd[Write]);
          start_process(fd[Read]);
        }
        else
        {
          // 父进程，等待子进程结束
          close(fd[Read]);
          close(fd[Write]);
          wait(&child_pid);
          exit(0);
        }
      }
    }
    else
    {
      // 读取到一个数，判断是否能整除
      has_chlid = True;
      if (num % my_prime != 0)
      {
        // 不能整除，将这个数写入管道传递给下一个进程
        write(fd[Write], &num, 4);
      }
      else
      {
        // 能整除，丢弃
        continue;
      }
    }
  }
}

int main(int argc, char* argv[]) {

  // 创建管道
  int fd[2];
  pipe(fd);
  // 将数字2到35输入到管道中，给第一个进程使用
  for (int i = 2; i <= 35; i++)
  {
    write(fd[Write], &i, 4);
  }
  close(fd[Write]);
  // 启动第一个进程
  start_process(fd[Read]);
  return 0;
}
