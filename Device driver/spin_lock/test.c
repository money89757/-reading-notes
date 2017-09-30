#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
int main(int argc, const char *argv[])
{
	int fd;
	
	pid_t pid;
	fd = open("/dev/demo",O_RDWR);
	if(fd == -1)
	{
		perror("open");
		return -1;
	}
	printf("open success\n");	
	
	sleep(5);
	close(fd);
	return 0;
}
