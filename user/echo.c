#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[])
{
  int i;

  for (i = 1; i < argc; i++) {
    // 将当前字符打印到终端
    write(1, argv[i], strlen(argv[i]));

    // 是否是最后一个字符
    if (i + 1 < argc) {
      write(1, " ", 1);
    }
    else {
      write(1, "\n", 1);
    }
  }
  exit(0);
}
