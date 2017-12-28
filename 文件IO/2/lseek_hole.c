#include<stdio.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
int main(int argc, const char *argv[])
{
	int fd;

	fd = open("2.txt",O_WRONLY | O_CREAT | O_TRUNC,0664);

	lseek(fd,10,SEEK_SET);//偏移的10个位置叫做空洞，本身不占用磁盘空间
//	write(fd,"d",1);
	return 0;
}
