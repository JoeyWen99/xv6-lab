struct stat; // 文件状态结构体
struct rtcdate; // 实时时钟时间结构体

// 系统调用
int fork(void); // 创建一个新进程
int exit(int) __attribute__((noreturn)); // 退出当前进程
int wait(int*); // 等待子进程结束
int pipe(int*); // 创建一个管道
int write(int, const void*, int); // 写入文件
int read(int, void*, int); // 读取文件
int close(int); // 关闭文件
int kill(int); // 终止进程
int exec(char*, char**); // 执行一个新程序
int open(const char*, int); // 打开文件
int mknod(const char*, short, short); // 创建一个设备节点
int unlink(const char*); // 删除文件
int fstat(int fd, struct stat*); // 获取文件状态
int link(const char*, const char*); // 创建一个硬链接
int mkdir(const char*); // 创建一个目录
int chdir(const char*); // 改变当前目录
int dup(int); // 复制文件描述符
int getpid(void); // 获取当前进程ID
char* sbrk(int); // 改变进程堆的大小
int sleep(int); // 使进程休眠
int uptime(void); // 获取系统运行时间

// ulib.c
int stat(const char*, struct stat*); // 获取文件状态
char* strcpy(char*, const char*); // 复制字符串
void *memmove(void*, const void*, int); // 移动内存块
char* strchr(const char*, char c); // 查找字符
int strcmp(const char*, const char*); // 比较字符串
void fprintf(int, const char*, ...); // 格式化输出到文件
void printf(const char*, ...); // 格式化输出到终端
char* gets(char*, int max); // 从终端读取字符串
uint strlen(const char*); // 获取字符串长度
void* memset(void*, int, uint); // 设置内存块
void* malloc(uint); // 分配内存
void free(void*); // 释放内存
int atoi(const char*); // 将字符串转换为整数
int memcmp(const void *, const void *, uint); // 比较内存块
void *memcpy(void *, const void *, uint); // 复制内存块