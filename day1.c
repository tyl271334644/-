1、数据类型
    基本数据类型：
        字符型：
            char       1个字节
            unsigned char 1个字节
        非字符型：
            短整型：
                short    2个字节
                unsigned short
            整型：
                int         2个字节（16位机）/4个字节（32位机）
                unsigned int
            长整型：
                long        4个字节（32位机）/8个字节（64位机）
                unsigned long
                long long
            浮点型（实型）：
                float:单精度     4个字节
                double:双精度    8个字节
    构造数据类型：
        数组：int arr[5]; 分配连续的内存空间，数组元素的数据类型一致  
        指针：定义的变量用来存放地址，在同一机器里面任何指针所占内存大小一样
        结构体：
        枚举：
        共用体（联合体）：
    空类型 void


    32位机：
        int *a;   -->4个字节
        malloc(sizeof(int))
        char *b;  -->4个字节
    sizeof
    在C语言里面用数组操作字符串 "hello world" -->以'\0'结尾
linux一切皆文件
    普通文件    -
    目录文件    d
    块设备文件  b
    字符设备文件 c
    链接文件    l
    套接字文件  s
    管道文件    p

文件IO
    open read write close lseek
    文件描述符:在一个进程唯一标识一个文件
        顺序分配的非负整数(返回未使用的最小的)
        内核用以标识一个特定进程正在访问的文件
        其他资源(socket、pipe等)的访问标识
    标准输入、标准输出和标准错误
        由shell默认打开，分别为0/1/2，但是我们可以手动关闭
1/open函数
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int open(const char *pathname, int flags);
int open(const char *pathname, int flags, mode_t mode);  //创建新文件时使用
入参：
	pathname: 打开文件的路径名称
	flags: 文件的打开方式
	mode:  打开文件的权限   	用八进制表示	 注意umask
返回值：
	成功：新的文件描述符（）    失败：-1，还会返回error number   erron变量定义在error.h文件中
flags:  多个常量可用“或”运算构成。在<fcntl.h>中定义
	O_RDONLY：只读方式打开文件。
	O_WRONLY：可写方式打开文件。
	O_RDWR：读写方式打开文件。
		以上三个选项互斥，只能选一个，下面选项可选0~多个。
	O_EXEC：只执行打开（没用过）
	O_CREAT：如果该文件不存在，就创建一个新的文件，并用第三的参数为其设置权限，如果没有加mode，mode会是一个随机值。
	O_EXCL：如果使用O_CREAT时文件存在，则可返回错误消息。这一参数可测试文件是否存在。
	O_NOCTTY：使用本参数时，如文件为终端，那么终端不可以作为调用open()系统调用的那个进程的控制终端。
	O_NOFOLLOW：如果path引用的是一个符号链接，则出错。
    O_NONBLOCK：如果path是一个FIFO、一个块特殊文件或一个字符特殊文件，则此选项为文件的本次打开操作和后续的I/O操作设置非阻塞方式。
	O_TRUNC：如文件已经存在，那么打开文件时先删除文件中原有数据。
	O_APPEND：以添加方式打开文件，所以对文件的写操作都在文件的末尾进行。
2/可用close函数关闭一个文件：
	#include<unistd.h>
	int close(int filedes);
	返回：若成功为0，若出错为-1；
	关闭一个文件时也释放该进程加在该文件上的所有记录锁。
	当一个进程终止时，它所有的打开文件都由内核自动关闭。
3/read函数
	从打开的文件中读数据。
	#include<unistd.h>
	read(int fd，void buff,size_t nbytes);
    入参：fd: 文件描述符
         buff：用来存储读到的数据
         nbytes：读的字节数
	返回值：读到的字节数  若读到文件末尾返回0  错误返回-1。
 4/write函数
	#include<unistd.h>
	ssize_t write(int fd, const void *buf, size_t count);
    入参：fd: 文件描述符
         buf：要写的数据
         count：要的字节数
    返回值 ： 成功 返回已写入的字节数  失败  返回-1。
mian传参：
    int main(int argc,char *argv[]);//argc   命令行传入的参数个数  argv就是参数，字符串

作业：
    实现一个文件拷贝（文件名从命令行传入）