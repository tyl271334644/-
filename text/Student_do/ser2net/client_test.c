#include <stdio.h>
#include <string.h>

#include <sys/socket.h>  
#include <sys/types.h>
#include <netinet/in.h>  
#include <arpa/inet.h>

#include <signal.h>
#include <unistd.h>

#define MSGMAX 5

int main()
{
	signal(SIGPIPE, SIG_IGN);

	/*创建套接字，用于连接*/
	int s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(0 > s){
		perror("socket");
		return -1;
	}

	/*绑定server IP及端口号*/
	struct sockaddr_in addr = {
		.sin_family = PF_INET,
		.sin_port   = htons(2001),
		.sin_addr   = {
			.s_addr =  inet_addr("192.168.1.1"),
		},
		.sin_zero = { 0 }
	};
	socklen_t len = sizeof(struct sockaddr_in);
	if(0 > connect(s, (struct sockaddr*)&addr, len)){
		perror("connect");
		goto ERR_STEP;
	}
	printf("connected\n");

	unsigned char cmd[][5] = {
#if 1
		{0xff,0x00,0x00,0x00,0xff},
		{0xff,0x00,0x01,0x00,0xff},
		{0xff,0x00,0x02,0x00,0xff},
		{0xff,0x00,0x03,0x00,0xff},
		{0xff,0x00,0x04,0x00,0xff},
		{0xff,0x00,0x05,0x00,0xff},
		{0xff,0x00,0x06,0x00,0xff},
		{0xff,0x00,0x07,0x00,0xff},
		{0xff,0x00,0x08,0x00,0xff}
#endif
	};
	unsigned char mode[] = {0xff,0x13,0x00,0x00,0xff};
	unsigned char lspeed[] = {0xff,0x02,0x01,0x09,0xff};
	unsigned char rspeed[] = {0xff,0x02,0x01,0x09,0xff};


#if 0
	if(MSGMAX != write(s, mode, MSGMAX)){
		perror("0-write");
		return -1;
	}
	if(MSGMAX != write(s, lspeed, MSGMAX)){
		perror("1-write");
		return -1;
	}
	if(MSGMAX != write(s, rspeed, MSGMAX)){
		perror("1-write");
		return -1;
	}
	printf("write done.\n");
#endif

#if 1	
	while(1){
		char ch = getchar();
		int id =0;
		switch(ch){
		case 's':
			id = 0;
			break;
		case '1':
			id = 1;
			break;
		case '2':
			id = 2;
			break;
		case '3':
			id = 3;
			break;
		case '4':
			id = 4;
			break;
		case '5':
			id = 5;
			break;
		case '6':
			id = 6;
			break;
		case '7':
			id = 7;
			break;
		case '8':
			id = 8;
			break;

		case 'q':
		case 'Q':
			return 0;

		default:
			continue;
		};

		if(MSGMAX != write(s, cmd[id], MSGMAX)){
			perror("2-write");
			return -1;
		}

		printf("cmd: ");
		for(int i = 0; i < MSGMAX; i++){
			printf("0x%x ", cmd[id][i]);
		}
		printf("\n");
#endif
	}

	return 0;

ERR_STEP:
	close(s);
	return -1;	
}
