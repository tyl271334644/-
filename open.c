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
int main()
{
    close(0);
    close(2);
    int ret = open("1.txt",O_RDONLY);
    int ret1 = open("a.out",O_RDONLY);
    int ret2 = open("2.txt",O_RDONLY|O_CREAT,0664);
    if(ret == -1|| ret1 == -1||ret2 == -1)
    {
        perror("open error");
        return -1;
    }
    printf("ret = %d\n",ret);
    printf("ret1 = %d\n",ret1);
    printf("ret2 = %d\n",ret2);
    return 0;
}
