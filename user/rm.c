#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[])
{

  // 如果命令行参数少于 2（即没有指定要删除的文件），则打印用法信息并退出。
  if (argc < 2) {
    fprintf(2, "Usage: rm files...\n");
    exit(1);
  }

  // 遍历所有要删除的文件。
  for (int i = 1; i < argc; i++) {
    // 尝试删除文件。如果 unlink 函数调用失败（返回值小于 0），则打印错误信息。
    if (unlink(argv[i]) < 0) {
      fprintf(2, "rm: %s failed to delete\n", argv[i]);
      break; // 停止删除其它文件
    }
  }

  exit(0); // 正常退出
}
