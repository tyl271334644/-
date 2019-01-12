#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include <sys/types.h>		//primitive system data types(包含很多类型重定义，如pid_t、int8_t等)   
#include <sys/socket.h>		//与套接字相关的函数声明和结构体定义，如socket()、bind()、connect()及struct sockaddr的定义等

#include <unistd.h>			//read,write
#include <netinet/in.h>	 	//某些结构体声明、宏定义，如struct sockaddr_in、PROTO_ICMP、INADDR_ANY等
#include <netinet/ip.h>	 
#include <arpa/inet.h>		//某些函数声明，如inet_ntop()、inet_ntoa()等

#include <error.h>			//perror

#include <sys/stat.h>
#include <fcntl.h>
#include "serial.h"


#define MAXLITE 50
#define devpath "/dev/ttyATH0"


typedef struct sockaddr SA;

int main(int argc, char *argv[])
{
	//串口初始化函数"/dev/ttyATH0"
	
	int fd = serial_init(devpath);
	if (fd < 0){
		perror("serial init:");
	}
	printf("fd = %d, serial init success\n", fd);
	
	int opt = 0;
	socklen_t client;
	struct sockaddr_in serveraddr, clientaddr;

	//创建套接字  自己完成
	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

	if(setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) < 0)	//端口重用	
	{
		perror("setsockport error");
		return -1;
	}
	bzero(&serveraddr, sizeof(serveraddr));
//填写网络三元组信息	
/*	servaddr.sin_family 		= PF_INET;*/
/*	servaddr.sin_addr.s_addr 	= inet_addr("127.0.0.1");*/
//	servaddr.sin_port			= htons(1000);
	serveraddr.sin_family	= AF_INET;	//TCP 协议			//填充地址信息
	//servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	//servaddr.sin_port			= htons(1000);
	serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
	serveraddr.sin_port = htons(atoi(argv[2]));
	//绑定地址信息  自己完成
	int ret = bind(socket_fd, (SA*)&serveraddr, sizeof(serveraddr));
	if(ret < 0)
	{
		perror("bind error");
		return -1;
	}
	//创建监听队列  自己完成
	ret = listen(socket_fd, 5);
	if(ret < 0)
	{
		perror("listen error");
		return -1;
	}
	
	while(1){
		printf("wait for a new client\n");
		client = sizeof(clientaddr);
		memset(&clientaddr, 0, sizeof(struct sockaddr_in));
		//建立连接请求  自己完成
		int new_fd = accept(socket_fd, (SA*)&clientaddr, &client);
		if(new_fd < 0)
		{
			perror("accecpt error");
			return -1;
		} 
		else 
		{
			printf("connection from %s, port %d\n", inet_ntoa(clientaddr.sin_addr),
					ntohs(clientaddr.sin_port));
			while(1){
				unsigned char buff[5] = { 0 };
				//从套接字接收  自己完成
				int len = read(new_fd, buff, sizeof(buff));
				if (len < 0)
				{
					perror("read error");
					return -1;
				}
				else if (len == 0)
				{
					break;
				}
				else 
				{
				      printf("0x%d\n",buff);
				      serial_send_data(fd,buff,len);
					  memset(buff,0,sizeof buff);
				}
			}
		}
		close(new_fd);
	}
	close(fd);
	close(socket_fd);
}
