/*************************************************************************
	> File Name: main.c
	> Author: 
	> Mail: 
	> Created Time: 2019年01月07日 星期一 17时18分51秒
 ************************************************************************/

#include<stdio.h>
int main(int argc,char *argv[])
{
    printf("argc = %d\n",argc);
    int i=0;
    for(i=0;i<argc;i++)
    {
        puts(argv[i]);        
    }
    return 0;
}
