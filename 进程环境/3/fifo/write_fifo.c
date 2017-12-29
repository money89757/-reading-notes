#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>

int main(int argc, const char *argv[])
{
#if 0
	int fd;

	fd = open("abc",O_WRONLY);
	printf("start\n");
	write(fd,"hello",6);
	printf("end\n");
#endif 
#if 0
	int fd;

	mkfifo("abc",0664);//本身自带了创建功能
	
	fd = open("abc",O_WRONLY);
	write(fd,"hello",6);
#endif 
	
	int fd;
	if(mkfifo("abc",0664 | O_EXCL) == -1)
	{
		if(errno == EEXIST)//说明有名管道是由对端创建
		{
			fd = open("abc",O_WRONLY);	
			write(fd,"hello",6);
		}
		else 
		{
			perror("mkfifo");
			return -1;
		}
	}
	else//有名管道有当前进程创建 
	{
		fd = open("abc",O_WRONLY);
		write(fd,"hello",6);
	}
	close(fd);
	return 0;
}
