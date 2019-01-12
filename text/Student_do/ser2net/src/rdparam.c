#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rdparam.h"

int readsetting(struct serverini *pini)
{
#define ENDFLAG '\n'
 
	struct serverini ini;
	memset(&ini, 0, sizeof(ini));

	FILE *fp =fopen("ser2net.conf", "r");
	if(NULL == fp){
		return -1;
	}

	static char devname[] = "/dev/ttyATHXXX\r\n";
	if(NULL == fgets(devname, sizeof(devname), fp))
		goto ERR_STEP;
	int len = strlen(devname);
	if(ENDFLAG != devname[len-1]){
		goto ERR_STEP;
	}
	devname[len-1] = 0;
	ini.vd = devname;

	char num[] = "xxxxxx\r\n";
	if(NULL == fgets(num, sizeof(num), fp))
		goto ERR_STEP;
	len = strlen(num);
	if(ENDFLAG != num[len-1]){
		goto ERR_STEP;
	}
	num[len-1] = 0;
	ini.port = atoi(num);

	if(NULL == fgets(num, sizeof(num), fp))
		goto ERR_STEP;
	len = strlen(num);
	if(ENDFLAG != num[len-1]){
		goto ERR_STEP;
	}
	num[len-1] = 0;
	ini.backlog = atoi(num);

	memcpy(pini, &ini, sizeof(ini));

ERR_STEP:
	fclose(fp);
	return 0;
}

