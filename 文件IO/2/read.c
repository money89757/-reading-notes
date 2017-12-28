#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

int main(int argc, const char *argv[])
{
	int fd,fd1;

	fd = open("1.txt",O_RDONLY);
	
	if(fd == -1)
	{

	}

	fd1 = open("2.txt",O_WRONLY | O_CREAT,0664);

	char buf[100];
	ssize_t n;
	n = read(fd,buf,sizeof(buf));
//	buf[n - 1] = '\0';
	printf("n = %d\n",n);
//	printf("buf = %s\n",buf);
//	write(fd1,buf,100);
	
	write(fd1,buf,n);//注意:参数3要以实际读取到的字节个数为准，不能以空间总大小为准
	close(fd);
	return 0;
}
