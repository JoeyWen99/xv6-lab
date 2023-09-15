#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char* path) //format name 的缩写，从路径中取出文件名并格式化文件名
{
  static char buf[DIRSIZ + 1];
  char* p;

  // Find first character after last slash.
  for (p = path + strlen(path); p >= path && *p != '/'; p--); // 从后往前找到第一个/的位置

  p++; // p指向最后一个/的下一个字符

  // Return blank-padded name. 
  if (strlen(p) >= DIRSIZ) 
    return p;
  memmove(buf, p, strlen(p));
  memset(buf + strlen(p), ' ', DIRSIZ - strlen(p)); // 末尾填充空格
  return buf;
}

void
ls(char* path) //传入的是一个目录名
{
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
    printf("%s %d %d %l\n", fmtname(path), st.type, st.ino, st.size); // 打印文件信息
    break;

  case T_DIR: // 如果是目录，递归调用ls
    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) { // 判断buf是否比文件路径长
      printf("ls: path too long\n"); 
      break;
    }

    strcpy(buf, path); // 路径存到 buf 中
    p = buf + strlen(buf); // 指向路径末尾
    *p++ = '/'; // 在路径末尾添加斜杠,buf中是当前路径

    //!重要: 已知打开的文件是目录时：只需不断地读struct dirent大小的数据即可得到目录中的文件名
    while (read(fd, &de, sizeof(de)) == sizeof(de)) { // 读取目录项
      if (de.inum == 0) // 如果目录项的inode号为0，跳过
        continue;

      memmove(p, de.name, DIRSIZ); // 复制目录项的名字
      p[DIRSIZ] = 0; // 添加字符串结束符

      if (stat(buf, &st) < 0) { // 获取文件状态
        printf("ls: cannot stat %s\n", buf); 
        continue;
      }
      printf("%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size); // 打印文件信息
    }
    break;
  }
  close(fd); // 关闭文件
}

int
main(int argc, char* argv[])
{
  int i;

  if (argc < 2) {
    ls(".");
    exit(0);
  }
  for (i = 1; i < argc; i++)
    ls(argv[i]);
  exit(0);
}
