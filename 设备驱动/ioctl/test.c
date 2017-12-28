#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/ioctl.h>
#include"myioctl.h"

//#define CMD1 100
//#define CMD2 200

struct test
{
	int a;
	char b;
}x = {
	120,
	'w'
};

int main(int argc, const char *argv[])
{
	int fd = open("/dev/demo0",O_RDWR);
	if(fd == -1)
	{

	}

//	ioctl(fd,CMD1);
//	ioctl(fd,CMD2);
	
	int value = 110;
	ioctl(fd,DEMO_CMD1);
	ioctl(fd,DEMO_CMD2,value);
	ioctl(fd,DEMO_CMD3,&x);
	ioctl(fd,DEMO_CMD4,&x);
	close(fd);
	return 0;
}
