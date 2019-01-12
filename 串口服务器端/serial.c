#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
       #include <sys/stat.h>
       #include <fcntl.h>
#include <stdio.h>
#include <string.h>
#if 0
int serial_init(char *devpath)
{
	int devfd = open(devpath, O_RDWR|O_NOCTTY);
	if(devfd < 0)
	{
		perror("open serial dev failed!");
		return -1;
	}
	struct termios termios;
	memset(&termios, 0, sizeof(termios));
/*	
	if(tcgetattr(devfd, &termios) != 0)
	{
		printf("get termios attr failed!\n");
		return -1;
	}
*/	
	
	termios.c_cflag |= CLOCAL | CREAD;//不占用端口，可以读取输入字符
	//termios.c_iflag = IGNPAR;
	termios.c_cflag |= CS8;		//设置数据位为8
	//termios.c_cflag &= ~CSIZE;
	termios.c_cflag &=~PARENB;		//设置无校验
	//termios.c_iflag &=~INPCK;
	termios.c_cflag &= ~CSTOPB;		//设置停止位为1
	termios.c_cflag &= ~CRTSCTS;	//不使用硬件流控
	cfsetispeed(&termios, B9600);	//设置波特率为115200
	cfsetospeed(&termios, B9600);
	termios.c_cc[VTIME] = 10;
	termios.c_cc[VMIN] = 0;
	termios.c_oflag = 0;
	
	
	if(tcflush(devfd, TCIOFLUSH) != 0)
	{
		printf("discard I/O buffer data failed!\n");
	}
	
	if(tcsetattr(devfd, TCSANOW,&termios) != 0)
	{
		perror("set termios attr failed!");
		return -1;
	}
	
	
	sleep(2);
	return devfd;
}
#endif
#if 1
int serial_init(char *devpath)
{
	int devfd = open(devpath, O_RDWR|O_NOCTTY|O_NDELAY);
	if(devfd < 0)
	{
		perror("open serial dev failed!");
		return -1;
	}
	struct termios termios;
	memset(&termios, 0, sizeof(termios));
/*	
	if(tcgetattr(devfd, &termios) != 0)
	{
		printf("get termios attr failed!\n");
		return -1;
	}
*/	
	
	termios.c_cflag |= CLOCAL | CREAD;//不占用端口，可以读取输入字符
	//termios.c_iflag = IGNPAR;
	termios.c_cflag |= CS8;		//设置数据位为8
	termios.c_cflag &= ~CSIZE;
	termios.c_cflag &=~PARENB;		//设置无校验
	//termios.c_iflag &=~INPCK;
	termios.c_cflag &= ~CSTOPB;		//设置停止位为1
	termios.c_cflag &= ~CRTSCTS;	//不使用硬件流控
	termios.c_cflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	termios.c_oflag &= ~OPOST; 
	cfsetispeed(&termios, B9600);	//设置波特率为115200
	cfsetospeed(&termios, B9600);
	//termios.c_cc[VTIME] = 10;
	//termios.c_cc[VMIN] = 0;
	//termios.c_oflag = 0;
	
	
	if(tcflush(devfd, TCIOFLUSH) != 0)
	{
		printf("discard I/O buffer data failed!\n");
	}
	
	if(tcsetattr(devfd, TCSANOW,&termios) != 0)
	{
		perror("set termios attr failed!");
		return -1;
	}
	
	
	sleep(2);
	return devfd;
}

#endif
int serial_read_data(int fd, unsigned char *data_buf, int data_size)
{
	int count = 0;
	int ret = 0;

	while(count < data_size)
	{
		printf("serial read!\n");
		ret = read(fd, data_buf+count, data_size - count);
		if(data_buf[0] != 0xbb)
		{
			if(tcflush(fd, TCIOFLUSH)!= 0)
				printf("flush serial_data_buffer fialed!\n");
			break;
		}
		
		
		
			if(ret == 0)
			{
				printf("read timeout or serial_data_buffer have no data!\n");
				continue;
			}
			else if(ret == -1)
			{
				perror("read error");
				return -1;
			}
			else
			{
				count += ret;
			}
	}
	return count;
}
int serial_send_data(int fd, unsigned char *data_buf, int data_size)
{
	int count = 0;
	int ret = 0;

	while(count < data_size)
	{
		printf("serial write!\n");
		ret = write(fd, data_buf+count, data_size - count);	
			if(ret == 0)
			{
				printf("write timeout\n");
				continue;
			}
			else if(ret == -1)
			{
				perror("write error");
				return -1;
			}
			else
			{
				count += ret;
			}
	}
	return count;
}
#if 0
void main()
{
	int m0_fd = serial_init("/dev/ttyUSB0");
	if(m0_fd == -1)
	{
		return;
	}
	printf("init serial success!\n");
	
	unsigned char recvbuf[36] = {0};
	while(1)
	{
		memset(recvbuf, 0, 36);
		serial_read_data(m0_fd, recvbuf, 36);
		
		
		int i;
		for(i = 0; i < 36; i++)
		{
			printf("%x ", recvbuf[i]);
		}
		printf("\n");
		
		
		
		unsigned char led_cmd0[36] ={0xdd, 0x06, 0x24, 0x00, 0x00};
	   	int ret = serial_send_data(m0_fd, led_cmd0, 36);
		printf("write ret = %d\n", ret);
		sleep(4);
		unsigned char led_cmd1[36] ={0xdd, 0x06, 0x24, 0x00, 0x01};
	   	ret = serial_send_data(m0_fd, led_cmd1, 36);
		printf("write ret = %d\n", ret);
		sleep(4);
		
		
	}
	return;
}
#endif


