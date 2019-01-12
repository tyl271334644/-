IP：在网络中，唯一标识一台主机；
port：在网络中的一台主机中，唯一标识一个进程；
IP+PORT:在网络中唯一标识一个进程

网络字节序：（IBM）
	小端序：  高地址存高字节，低地址存低字节（PC本地存储）
	大端序：  高地址存低字节，低地址存高字节（网络存储）
	TCP/IP协议规定，网络数据流应采用大端字节序
    为使网络程序具有可移植性，使同样的C代码在大端和小端计算机上编译后都能正常运行，可以调用以下库函数做网络字节序和主机字节序的转换。



1/	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	uint32_t htonl(uint32_t hostlong); 本地-->网络（IP）
	uint16_t htons(uint16_t hostshort);本地-->网络（port）
	uint32_t ntohl(uint32_t netlong);
	uint16_t ntohs(uint16_t netshort);
	h表示host，n表示network，l表示32位长整数，s表示16位短整数。
IP地址转换函数：
2/	#include <arpa/inet.h>
	int inet_pton(int af, const char *src, void *dst);本地字节序到网络字节序
	af:用来制定协议类型 AF_INET、AF_INET6
	src:传入IP地址（点分十进制）
	dst:传出，转换后的网络字节序的IP地址
返回值：
	成功：1
	异常：0，说明src指向的不是一个有效的地址。
	失败：-1
3/	const char *inet_ntop(int af, const void *src,char *dst, socklen_t size);网络字节序
	到本地字节序 
	af:用来制定协议类型 AF_INET、AF_INET6
	src:网络字节序的IP地址
	dst:本地字节序IP地址（点分十进制）
	size:dst的大小。
	返回值：成功：dst
	失败：NULL
socket 地址结构
struct sockaddr {
     sa_family_t sa_family;
     char        sa_data[14];
}
struct sockaddr_in {
	sa_family_t    sin_family; /* address family: AF_INET */
	in_port_t      sin_port;   /* port in network byte order */
	struct in_addr sin_addr;   /* internet address */
};   //   man 7 ip
/* Internet address. */
struct in_addr {
	uint32_t       s_addr;     /* address in network byte order */
};	
	
	(man 7 ip)
1、创建套接字
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
int socket(int domain, int type, int protocol);
domain：AF_INET、AF_INET6、AF_UNIX
type：SOCK_STREAM、SOCK_DGRAM
protocol：0（默认流式套接字为TCP协议、报式套接字为UDP协议）
返回值：成功返回文件描述符（套接字）
		失败返回-1
tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
udp_socket = socket(AF_INET, SOCK_DGRAM	, 0);
2、绑定IP和prot
 #include <sys/types.h>          /* See NOTES */
 #include <sys/socket.h>
int bind(int sockfd, const struct sockaddr *addr,socklen_t addrlen);
sockfd：socket函数的返回值
	struct sockaddr_in addr;
	addr.sin_famliy = AF_INET;
	addr.sin_port = htons(9527);
	addr.sin_addr.s_addr =htonl( INADDR_ANY);//htonl( "192.168.0.123")
addr：（struct sockaddr *）&addr
addrlen：sizeof(addr)
返回值：成功返回0，失败返回-1 
bind(tcp_socket, (struct sockaddr *)&addr, sizeof(addr));		/* initialize addr */
3、设置监听上限（同时建立连接上限数）
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
int listen(int sockfd, int backlog);
sockfd：socket函数的返回值
backlog：上限数，最大数值128
返回值：成功返回0，失败返回-1 
4、阻塞等待客户端建立连接
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
sockfd：socket函数的返回值
addr：（传出参数）。成功与服务器建立连接的客户端信息
addrlen：传入传出。入：sizeof(addr) 出：客户端addr是的实际大小
返回值：成功返回能与服务器进行数据通信的新文件描述符。失败返回-1 
5、读数据
#include <unistd.h>
ssize_t read(int fd, void *buf, size_t count);
6、写数据
#include <unistd.h>
ssize_t write(int fd, const void *buf, size_t count);
7、客户端发送连接请求
 #include <sys/types.h>          /* See NOTES */
 #include <sys/socket.h>
 int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
sockfd：socket函数的返回值（客户端）
addr：传入参数，服务器的地址结构
addrlen：sizeof(addr)
返回值：成功返回0，失败返回-1
如果不使用绑定客户端地址结构，采用“隐式绑定”
	
	
TCP通信流程分析：
server:
	1、socket()  创建socket
	2、bind()    绑定服务器地址结构
	3、listen()  设置监听上限
	4、accept()  阻塞等待客户端连接
	5、read()    读socket获取客户端的数据
	6、write()   向socket写数据给客户端
	7、close()   关闭socket
client:
	1、socket()  创建socket
	//2、bind()   绑定客户端地址结构（可不写）
	3、connect() 与服务器建立连接 
	4、read()    读socket获取服务器的数据
	5、write()   向socket写数据给服务器
	6、close()   关闭socket

	
nc:net client   可以测试server可否通信
nc ip  prot  // nc 127.0.0.1 8888	
	
	
	
	
	
	
	
	
	
	
	