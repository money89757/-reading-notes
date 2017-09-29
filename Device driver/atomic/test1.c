#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

int main(int argc, const char *argv[])
{
	int fd;

	fd = open("/dev/demo",O_RDWR);

	if(fd == -1)
	{
		perror("open");
		return -1;
	}
	getchar();	
	close(fd);
	return 0;
}
