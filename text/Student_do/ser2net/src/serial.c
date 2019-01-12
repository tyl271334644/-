/*
 * writed by panzhh
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h> 

#include <string.h>
#include <errno.h>
extern int errno;

#include <poll.h>
#include <sys/ioctl.h>

#include "serial.h"

int init_serial(char *serialname, speed_t speed, char parity, int databits, int stopbits)
{
	int serialfd = open(serialname, O_RDWR|O_NONBLOCK|O_NOCTTY|O_NDELAY);
	if(-1 == serialfd){
		perror("open");
		return -1;
	}

	//if(0 == set_flags(serialfd))
		if(0 == set_speed(serialfd, speed))
			if(0 == set_parity(serialfd, parity))
				if(0 == set_databits(serialfd, databits))
					if(0 == set_stopbits(serialfd, stopbits))
						return serialfd;

	close(serialfd);
	return(-1);
}

int set_flags(int fd)
{
	struct termios opt;
	memset(&opt, 0, sizeof(struct termios));
	cfmakeraw(&opt);

	/* set raw input, 1 second timeout */
	opt.c_cflag |= (CLOCAL | CREAD);
	opt.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	opt.c_oflag &= ~OPOST;
	opt.c_oflag &= ~(ONLCR | OCRNL);
	opt.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
	opt.c_iflag &= ~(ICRNL | INLCR);
	opt.c_iflag &= ~(IXON | IXOFF | IXANY);

#if 0
	opt.c_cflag |= IXON|IXOFF|IXANY;	/*软件数据流控制*/
#else
	opt.c_cflag &= ~(IXON|IXOFF|IXANY);	/*不使用软件数据流控制*/
#endif

#if 0
	opt.c_cflag |= CRTSCTS;   		/*硬件数据流控制*/
#else
	opt.c_cflag &= ~CRTSCTS;		/*不使用数据流控制*/
#endif

	opt.c_cc[VTIME]= 10;
	opt.c_cc[VMIN] = 0;
	tcflush(fd, TCIFLUSH);

	if(0 > tcsetattr(fd, TCSANOW, &opt)){
		return (-1);
	}

	int status;
	ioctl(fd, TIOCMGET, &status);
#ifdef SERIALDBG
	printf("status = %x\n", status);
#endif
#if 0
	status &= ~(TIOCM_RTS);	/*RTS 引脚高电平*/
#else
	status |= TIOCM_RTS;	/*RTS 引脚低电平*/
#endif
#ifdef SERIALDBG
	printf("status = %x\n", status);
#endif
	ioctl(fd, TIOCMSET, &status);

#ifdef SERIALDBG
	ioctl(fd, TIOCMGET, &status);
	printf("status = %x\n", status);
#endif

	return 0;
}

int set_speed(int fd, speed_t speed)
{
	struct termios opt;
	if(-1 == tcgetattr(fd, &opt)){
		puts("tcgetattr");
		goto ERR_EXIT;
	}

	if(-1 == cfsetispeed(&opt, speed)){
		puts("cfsetispeed");	 
		goto ERR_EXIT;
	}

	if(-1 == cfsetospeed(&opt, speed)){
		puts("cfsetospeed");	
		goto ERR_EXIT;
	}

	if(-1 == tcsetattr(fd, TCSANOW, &opt)){
		puts("tcsetattr");	
		goto ERR_EXIT;
	}
	return 0;

ERR_EXIT:
	perror("set_speed");
	return (-1);
}
 
int set_parity(int fd, char parity)
{
	struct termios opt;

	if(-1 == tcgetattr(fd, &opt)){
		goto ERR_EXIT;
	}

	switch (parity){
	case 'n':
	case 'N':
		opt.c_cflag &= ~PARENB;		/* Clear parity enable */
		opt.c_iflag &= ~INPCK;		/* Enable parity checking */ 
		break;

	case 'o':
	case 'O':
		opt.c_cflag |= (PARODD | PARENB);	/* 设置为奇校验*/
		opt.c_iflag |= INPCK;			/* Disnable parity checking */ 
		break;

	case 'e':
	case 'E':   
		opt.c_cflag |= PARENB;		/* Enable parity */    
		opt.c_cflag &= ~PARODD;		/* 转换为偶校验*/     
		opt.c_iflag |= INPCK;		/* Disnable parity checking */
		break;

	case 'S': 
	case 's':  /*as no parity*/   
		opt.c_cflag &= ~PARENB;
		opt.c_cflag &= ~CSTOPB;
		break;

	default:
		goto ERR_EXIT;
	};

	if(-1 == tcsetattr(fd,TCSANOW,&opt)){
		goto ERR_EXIT;
	}

	return 0;

ERR_EXIT:
	perror("set_parity");
	return (-1);
}
 
int set_databits(int fd, int databits)
{
	struct termios opt;

	if(-1 == tcgetattr(fd, &opt)){
		goto ERR_EXIT;
	}

	opt.c_cflag &= ~CSIZE; 
	switch (databits)
	{   
	case 7:     
		opt.c_cflag |= CS7; 
		break;
	case 8:     
		opt.c_cflag |= CS8;
		break;   

	default:
		goto ERR_EXIT;
	};

	if(-1 == tcsetattr(fd,TCSANOW,&opt)){
	goto ERR_EXIT;
	}

	return 0;

ERR_EXIT:
	perror("set_databits");
	return (-1);
}

int set_stopbits(int fd, int stopbits)
{
	struct termios opt;

	if(-1 == tcgetattr(fd, &opt)){
		goto ERR_EXIT;
	}

	switch (stopbits){
	case 1:
		opt.c_cflag &= ~CSTOPB;  
		break;

	case 2:
		opt.c_cflag |= CSTOPB;
		break;

	default:
		goto ERR_EXIT;
	};

	if(-1 == tcsetattr(fd,TCSANOW,&opt)){
		goto ERR_EXIT;
	}

	return 0;

ERR_EXIT:
	perror("set_stopbits");
	return (-1);
}

int serialflush(int fd)
{
	return tcflush(fd, TCIFLUSH);
}

void release_serial(int fd)
{
	close(fd);
}

int send_serial(int fd, void *p, size_t count, int timeout)
{
#if 1
	static struct pollfd fds;
	fds.fd    = fd;
	fds.events= POLLOUT;	
	fds.revents=0;	

	/*60 second timeout*/
	int ret = poll(&fds, 1, timeout);
	if(0 >= ret)
	{
		perror("[write_serial]poll");
		return -1;	/*error or timout*/
	}

	if(fds.revents & POLLOUT)
		return write(fd, p, count);

	return -1;		/*unknow error*/
#else
	return write(fd, p, count);
#endif
}

int recv_serial(int fd, void *p, size_t count, int timeout)
{
#if 1
	static struct pollfd fds;
	fds.fd    = fd;
	fds.events= POLLIN;
	fds.revents=0;

	/*60 second timeout*/
	int ret = poll(&fds, 1, timeout);
	if(0 >= ret){
		perror("[recv_serial]poll");
		return -1;	/*error or timout*/
	}

	if(fds.revents & POLLIN)
		return read(fd, p, count);

	printf("[TRACE][recv_serial]poll unkonw err.\r\n");
	return -1;		/*unknow error*/
#else
	return read(fd, p, count);
#endif
}

