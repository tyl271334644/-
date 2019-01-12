/*
 * writed by panzhh
 * 2013.11.15
 */
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>

#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <linux/tcp.h>

#include <fcntl.h>  
#include <unistd.h>
#include <signal.h>

#include <stdio.h>  
#include <string.h>
#include <stdlib.h>

#include <errno.h>

#include "myserver.h"

/*
 * Name: init_signals
 * Description: Sets up signal handlers for all our friends.
 */
void init_signals(void)
{
	struct sigaction sa;

	memset(&sa, 0, sizeof(sa));
	sa.sa_flags = 0;

	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask, SIGPIPE);
	sigaddset(&sa.sa_mask, SIGUSR1);
	sigaddset(&sa.sa_mask, SIGUSR2);

	sa.sa_handler = SIG_IGN;
	sigaction(SIGPIPE, &sa, NULL);

	sa.sa_handler = SIG_IGN;
	sigaction(SIGUSR1, &sa, NULL);

	sa.sa_handler = SIG_IGN;
	sigaction(SIGUSR2, &sa, NULL);
}

int init_server(unsigned short port, int backlog)
{
	/*创建套接字，用于连接*/  //do it 1
	int s = socket(?);
	if(0 > s){
		perror("socket");
		return -1;
	}

	/*端口重用*/
	int reuseaddr = 1;
	if(0 > setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr))){
		perror("can't reuse addr on rw socket!");
		return -1;
	}

	/*绑定server IP及端口号*/
	struct sockaddr_in addr = {  //do it 2
		.sin_family = ,
		.sin_port   = ,
		.sin_addr   = {
			.s_addr =  INADDR_ANY,
		},
		.sin_zero = { 0 }
	};
	socklen_t len = sizeof(struct sockaddr_in);
													//do it 3
	if(0 > bind(?){  
		perror("bind");
		goto ERR_STEP;
	}

	/*设置监听数*/
													//do it 4
	if(0 > listen(?)){
		perror("listen");
		goto ERR_STEP;
	}

	printf("Wait for a new client entry.\n");
	return s;

ERR_STEP:
	close(s);
	return -1;
}

