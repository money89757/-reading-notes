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

	pid = fork();

	if(pid == -1)
	{

	}
	else if(pid == 0)//child
	{
		sleep(1);
		printf("*************write1************\n");
		write(fd,"hello",5);
		printf("*************write2************\n");
	}
	else //
	{
		printf("*************read1************\n");
		char buf[10];
		read(fd,buf,sizeof(buf));
		printf("read:%s\n",buf);
		printf("*************read2************\n");

	}
	close(fd);
	return 0;
}
