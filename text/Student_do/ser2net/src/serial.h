/*
 * writed by panzhh
 */
#ifndef _SERIAL_H_
#define _SERIAL_H_

#include <termios.h> 

int init_serial(char *serialname, speed_t speed, char parity, int databits, int stopbits);
int set_flags(int fd);
int set_speed(int fd, speed_t speed);
int set_parity(int fd, char parity);
int set_databits(int fd, int databits);
int set_stopbits(int fd, int stopbits);
void release_serial(int fd);

int serialflush(int fd);

int send_serial(int fd, void *p, size_t count, int timeout);
int recv_serial(int fd, void *p, size_t count, int timeout);

#endif
