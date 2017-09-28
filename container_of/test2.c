#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

int main(int argc, const char *argv[])
{
	int fd;

	fd = open("/dev/demo2",O_RDWR);

	if(fd == -1)
	{
		perror("open");
		return -1;
	}

	int n;
	n = write(fd,"farsight-shanghai",17);
	if(n == -1)
	{
		perror("write");
		return -1;
	}

	char buf[17];
	n = read(fd,buf,sizeof(buf));
	if(n == -1)
	{
		perror("read");
		return -1;
	}
	printf("ps3 copy from kernel:%s\n",buf);
	while(1);
	close(fd);
	return 0;
}
