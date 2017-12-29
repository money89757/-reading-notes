#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>

int main(int argc, const char *argv[])
{
	int fd;
	char buf[10];
	ssize_t n;
#if 0
	fd = open("abc",O_RDONLY);

	n = read(fd,buf,sizeof(buf));
	printf("%s\n",buf);
#endif

	if(mkfifo("abc",0664 | O_EXCL) == -1)
	{
		if(errno == EEXIST)
		{
			fd = open("abc",O_RDONLY);
			n = read(fd,buf,sizeof(buf));
			printf("%s\n",buf);
		}
		else 
		{
			perror("mkfifo");
			return -1;
		}
	}
	else 
	{
		fd = open("abc",O_RDONLY);
		n = read(fd,buf,sizeof(buf));
		printf("%s\n",buf);
	}
	
	close(fd);
	return 0;
}
