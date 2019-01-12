/*************************************************************************
	> File Name: open.c
	> Author: 
	> Mail: 
	> Created Time: 2019年01月07日 星期一 16时08分23秒
 ************************************************************************/

#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<unistd.h>
#include<string.h>
int main()
{
   int ret3=0;
    int ret = open("1.txt",O_RDONLY);
    int ret2 = open("2.txt",O_WRONLY);
    char buf[512]={0}; 
    if(ret == -1||ret2 == -1)
    {
        perror("open error");
        return -1;
    }
    printf("ret = %d\n",ret);
    while((ret3 = read(ret,buf,100))!=0)
    {
        write(1,buf,ret3);
        memset(buf,0,sizeof buf);
    }
    printf("read over\n");
    //int num = read(ret,buf,10);
    //printf("num = %d\nbuf=%s\n",num,buf);
    return 0;
}
