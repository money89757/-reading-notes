#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/ioctl.h>
#include"led_ioctl.h"

int main(int argc, const char *argv[])
{
	int fd;

	fd = open("/dev/led",O_RDWR);


	while(1)
	{
		ioctl(fd,LED2_ON);
		sleep(1);
		ioctl(fd,LED1_ON);
		sleep(1);
		ioctl(fd,LED3_ON);
		sleep(1);
	}
	return 0;
}
