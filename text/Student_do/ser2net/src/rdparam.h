#ifndef RDPARAM_H
#define PDPARAM_H

struct serverini{
	char *vd;
	unsigned short port;
	int backlog;
};

int readsetting(struct serverini *pini);

#endif
