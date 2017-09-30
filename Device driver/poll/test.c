#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/select.h>
int main(int argc, const char *argv[])
{
	int fd;
	int maxfd;
	int n;
	fd_set readfds;
	fd_set writefds;
	char buf[10];


	fd = open("/dev/demo",O_RDWR);
	if(fd == -1)
	{
		perror("open");
		return -1;
	}
	
	maxfd = fd + 1;

	while(1)
	{
		FD_ZERO(&readfds);
		FD_ZERO(&writefds);
		FD_SET(fd,&readfds);
		FD_SET(fd,&writefds);

		n = select(maxfd,&readfds,&writefds,NULL,NULL);

		if(FD_ISSET(fd,&readfds))
		{
			printf("*******read1*********\n");	
			read(fd,buf,10);
			printf("read:%s\n",buf);
			printf("*******read2*********\n");
		}

		if(FD_ISSET(fd,&writefds))
		{
			printf("*****write1*******\n");
			write(fd,"hello",5);
			printf("*****write2*******\n");
		}
		sleep(1);
	}
	return 0;
}
