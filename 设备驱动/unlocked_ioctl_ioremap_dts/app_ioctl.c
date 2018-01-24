#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include "fs4412_ioctl.h"

#define GLOBALSIZE 128
#define PATHNAME "/dev/ex"

int main(int argc, const char *argv[])
{
	int fd = -1;
	int ret = 0;
	fd = open(PATHNAME,O_RDWR);
	char buf[GLOBALSIZE] = {};
	if(fd < 0){
		perror("open failed");
		return -1;
	}
	printf("open has done!\n");

	ioctl(fd,LED_ON);
	sleep(1);
	ioctl(fd,LED_OFF);

	close(fd);
	printf("closed\n");
	return 0;
}
