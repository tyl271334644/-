/*************************************************************************
	> File Name: server.c
	> Author: 
	> Mail: 
	> Created Time: 2018年10月28日 星期日 11时09分18秒
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<string.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<arpa/inet.h>
#define SPORT 10086
int main()
{
    struct sockaddr_in saddr,caddr;
    char buf[100]={0};
    char buf_send[100]={0};
    socklen_t c_len;
    //创建socket套接字
    int sock_fd=socket(AF_INET,SOCK_STREAM,0);
    //绑定ip和port
    memset(&saddr,0,sizeof saddr);
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(SPORT);
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);//NADDR_ANY  自动获取本机IP
    int ret = bind(sock_fd,(struct sockaddr *)&saddr,sizeof saddr);
    if(ret == -1)
    {
        perror("bind error");
        return -1;
    }
    ret = listen(sock_fd,5);
    if(ret == -1)
    {
        perror("listen error");
        return -1;
    }
    int new_fd=accept(sock_fd,(struct sockaddr *)&caddr,&c_len);
    if(new_fd == -1)
    {
        perror("accept error");
        return -1;
    }
    printf("accept ok new_fd = %d\n",new_fd);
    while(1)
    {
        ret = read(new_fd,buf,sizeof buf);
        if(ret == -1)
        {
            perror("read error");
            return -1;
        }
        printf("%s\n",buf);
        gets(buf_send);
        write(new_fd,buf_send,strlen(buf_send));
    }
    close(new_fd);
    close(sock_fd);
    return 0;
}
