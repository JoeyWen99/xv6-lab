#include "kernel/types.h"
#include "user/user.h"

enum { Read, Write };
enum { False, True };

void start_process(int readfd) {
  int num; // 读入的数据
  int my_prime = -1; // 本进程的质数
  int child_pid; // 子进程的pid

  int fd[2]; // 管道
  pipe(fd);

  read(readfd, &my_prime, 4); // 读的第一个数设置为my_prime,并打印
  printf("prime %d\n", my_prime);

  if (read(readfd, &num, 4) == 0) // 判断一下是否是叶子节点
    exit(0);

  do { // 从管道内不断地读入数据并处理
    if (num % my_prime != 0)
    {
      write(fd[Write], &num, 4); // 不能整除，将这个数写入管道传递给下一个进程
    }
    else
    {
      continue; // 能整除，丢弃
    }
  } while (read(readfd, &num, 4) != 0);


  close(readfd); // 传入管道内的数据已读完，关闭读入端
  close(fd[Write]); // 需要写入的数据全部丢入管道，关闭写入端

  child_pid = fork(); // 非叶子结点，创建进程处理管道内数据

  if (child_pid == 0) // 子进程，启动下一个进程
  {
    start_process(fd[Read]);
  }
  else // 父进程，等待子进程结束
  {
    close(fd[Read]);
    wait(&child_pid);
    exit(0);
  }
}

int main(int argc, char* argv[]) {

  int fd[2];
  pipe(fd); // 创建管道

  for (int i = 2; i <= 35; i++) // 将数字2到35输入到管道中，给第一个进程使用
  {
    write(fd[Write], &i, 4);
  }
  close(fd[Write]);

  start_process(fd[Read]); // 启动第一个进程
  return 0;
}
