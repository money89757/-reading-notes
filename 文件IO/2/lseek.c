#include<stdio.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
int main(int argc, const char *argv[])
{
	int fd;

	fd = open("1.txt",O_WRONLY | O_CREAT | O_TRUNC,0664);

	lseek(fd,10,SEEK_SET);
	write(fd,"a",1);

	lseek(fd,-4,SEEK_END);
	write(fd,"b",1);
	write(fd,"c",1);

	lseek(fd,10,SEEK_CUR);
	write(fd,"d",1);
	return 0;
}
