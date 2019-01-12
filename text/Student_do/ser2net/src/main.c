#include <stdio.h>
#include <string.h>

#include <sys/socket.h>  
#include <sys/types.h>
#include <netinet/in.h>  
#include <arpa/inet.h>

#include <unistd.h>
#include <termios.h>

#include "myserver.h"
#include "rdparam.h"
#include "serial.h"

#define MSGMAX 5

int main()
{
	struct serverini ini;

	ini.vd = "/dev/ttyATH0";
	ini.port = 2001;
	ini.backlog = 9;

	readsetting(&ini);
	printf("serial: %s\n", ini.vd);
	printf("port=%d, backlog=%d\n", ini.port, ini.backlog);

	int fd = init_serial(ini.vd, B9600, 'n', 8, 1);
	if(0 > fd){
		perror("init_serial");
		return -1;
	}

	init_signals();

	int s = init_server(ini.port, ini.backlog);
	if(0 > s){
		return -1;
	}

	while(1){
		struct sockaddr_in addr;
		memset(&addr, 0, sizeof(addr));
		socklen_t len = sizeof(struct sockaddr_in);
														//do it 5
		int rws = accept();
		if(0 > rws){
			perror("ser2net:accept");
			break;
		}
		printf("new comming: %s:%d\n", 
			inet_ntoa(addr.sin_addr),
			ntohs(addr.sin_port));

		while(1){
			unsigned char cpack[MSGMAX];
			memset(cpack, 0, MSGMAX);
			int num = read(rws, cpack, MSGMAX);
			if(MSGMAX != num){
				printf("message pack is err.\n");
				close(rws);
				break;
			}else	if((0xFF!=cpack[0]) || (0xFF!=cpack[4])){
				printf("message pack is err.\n");
				close(rws);
				break;
			}

			printf("cmd: ");
			for(int i = 0; i < MSGMAX; i++){
				printf("0x%x ", cpack[i]);
			}
			printf("\n");
	
			if(0 > send_serial(fd, cpack, MSGMAX, 600)){
				perror("send serial fail");
				close(rws);
				break;
			}
			printf("send_serial[%d] done.\n", fd);
		}
	}

	close(s);

	return 0;
}
