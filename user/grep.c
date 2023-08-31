// Simple grep.  Only supports ^ . * $ operators.

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// 匹配缓冲区
char buf[1024];

// 函数原型声明
int match(char*, char*);

// 主搜索函数
void grep(char* pattern, int fd) {
  int n, m;
  char* p, * q;

  m = 0; // m 记录缓冲区中有效数据的长度
  // 从文件描述符 fd 中读取数据到缓冲区 buf
  while ((n = read(fd, buf + m, sizeof(buf) - m - 1)) > 0) {
    m += n; // 更新缓冲区中有效数据的长度
    buf[m] = '\0'; // 在缓冲区末尾添加 null 终止符
    p = buf; // p 指向缓冲区开始位置

    // 遍历缓冲区中的每一行
    while ((q = strchr(p, '\n')) != 0) {
      *q = 0; // 替换行尾的 '\n' 为 '\0'
      // 如果该行与 pattern 匹配
      if (match(pattern, p)) {
        *q = '\n'; // 恢复行尾的 '\n'
        write(1, p, q + 1 - p); // 输出该行
      }
      p = q + 1; // p 指向下一行的开始位置
    }

    // 移动剩余的不完整行到缓冲区开始
    if (m > 0) {
      m -= p - buf;
      memmove(buf, p, m);
    }
  }
}

// 主函数
int main(int argc, char* argv[]) {
  int fd, i;
  char* pattern;

  // 判断命令行参数个数
  if (argc <= 1) {
    fprintf(2, "usage: grep pattern [file ...]\n");
    exit(1);
  }

  pattern = argv[1]; // 获取搜索模式

  // 如果没有指定文件，从标准输入读取
  if (argc <= 2) {
    grep(pattern, 0);
    exit(0);
  }

  // 遍历所有指定的文件
  for (i = 2; i < argc; i++) {
    // 打开文件
    if ((fd = open(argv[i], 0)) < 0) {
      printf("grep: cannot open %s\n", argv[i]);
      exit(1);
    }
    // 搜索文件
    grep(pattern, fd);
    close(fd); // 关闭文件
  }
  exit(0);
}


// Regexp matcher from Kernighan & Pike,
// The Practice of Programming, Chapter 9.

int matchhere(char*, char*);
int matchstar(int, char*, char*);

int
match(char* re, char* text)
{
  if (re[0] == '^')
    return matchhere(re + 1, text);
  do {  // must look at empty string
    if (matchhere(re, text))
      return 1;
  } while (*text++ != '\0');
  return 0;
}

// matchhere: search for re at beginning of text
int matchhere(char* re, char* text)
{
  if (re[0] == '\0')
    return 1;
  if (re[1] == '*')
    return matchstar(re[0], re + 2, text);
  if (re[0] == '$' && re[1] == '\0')
    return *text == '\0';
  if (*text != '\0' && (re[0] == '.' || re[0] == *text))
    return matchhere(re + 1, text + 1);
  return 0;
}

// matchstar: search for c*re at beginning of text
int matchstar(int c, char* re, char* text)
{
  do {  // a * matches zero or more instances
    if (matchhere(re, text))
      return 1;
  } while (*text != '\0' && (*text++ == c || c == '.'));
  return 0;
}

