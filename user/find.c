#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
get_filename(char* path) //format name 的缩写，从路径中取出文件名并格式化文件名
{
  static char buf[DIRSIZ + 1];
  char* p;
  // Find first character after last slash.
  for (p = path + strlen(path); p >= path && *p != '/'; p--); // 从后往前找到第一个/的位置
  p++; // p指向最后一个/的下一个字符
  memmove(buf, p, strlen(p));// 将文件名称复制到buf中
  return buf;
}
//找到相同名字的文件直接打印
void find(char* path, char* name) {

  char buf[512], * p;  // 缓冲区和指针
  int fd;              // 文件描述符
  struct dirent de;    // 目录项结构体
  struct stat st;      // 文件状态结构体

  if ((fd = open(path, 0)) < 0) {  // 查看传入的文件是否存在
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }

  if (fstat(fd, &st) < 0) { // 获取文件状态
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch (st.type) { // 判断文件类型
  case T_FILE: // 如果是文件
    if (strcmp(get_filename(path), name) == 0) {
      printf("%s\n", path);
    }
    break;

  case T_DIR: // 如果是目录，对于子目录递归调用find
    // printf("path:%s\n", path);
    strcpy(buf, path); // 路径存到 buf 中
    p = buf + strlen(buf); // 指向路径末尾
    *p++ = '/'; // 在路径末尾添加斜杠
    // printf("path:%s\n", buf);

    //!重要: 已知打开的文件是目录时：只需不断地读struct dirent大小的数据即可得到目录中的文件名
    while (read(fd, &de, sizeof(de)) == sizeof(de)) { // 读取目录项
      if (de.inum == 0)
        continue;
      if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) // 如果目录项的inode号为0或1，跳过
        continue;

      // printf("de.name: %s\n", de.name);
      memmove(p, de.name, DIRSIZ); // 复制目录项的名字
      p[DIRSIZ] = 0; // 添加字符串结束符

      // 找到了
      if (strcmp(de.name, name) == 0) {
        printf("%s\n", buf);
        continue;
      }

      if (stat(buf, &st) < 0) { // 获取文件状态
        printf("ls: cannot stat %s\n", buf);
        continue;
      }
      if (st.type == T_DIR)// 深度优先搜索
      {
        // printf("subdir_path:%s\n", buf);
        find(buf, name);
      }
    }
    break;
  }
  close(fd); // 关闭文件
}


int
main(int argc, char* argv[])
{
  if (argc != 3) {
    fprintf(2, "usage: find <path> <name>\n");
    exit(1);
  }
  else {
    find(argv[1], argv[2]);
    exit(0);
  }
}
