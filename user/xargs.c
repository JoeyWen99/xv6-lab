#include "kernel/types.h"
#include "user/user.h" //可用的系统调用
#include "kernel/param.h" 


int main(int argc, char* argv[]) {
  // find . b | xargs grep hello
  // 先执行find . b，将结果传给xargs，xargs再执行grep hello
  // argv[0] = xargs
  // argv[1] = grep
  // argv[2] = hello
  char* xargv[MAXARG] = { 0 }; //传入exce 的参数，初始化全为0

  // xargv[0] = grep; xargv[1] = hello
  for (int i = 1; i < argc; i++) {
    xargv[i - 1] = argv[i];
  }

  char buf[128] = { 0 };
  char* p = buf;
  //从标准输入中读入所有数据到 buf 中
  while (read(0, p, 1) > 0) {
    if (*p == '\n') {//如果读到换行符
      *p = '\0';//将换行符替换为字符串结束符
      if (fork() == 0) {//创建子进程
        xargv[argc - 1] = buf;//将buf作为参数传入
        exec(argv[1], xargv);//执行grep hello
      }
      else {
        wait(0);//等待子进程结束
      }
      p = buf;//重置p
    }
    else {
      p++;
    }
  }
  exit(0);
}
