#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

int main(int argc, const char *argv[])
{
	int fd,fd1,fd2,fd3;

//	fd = open("1.txt",O_RDONLY);
	umask(0123);//linux默认的umask为002
	fd = open("1.txt",O_RDONLY | O_CREAT,0664);
	if(fd == -1)
	{
		perror("open");
		return -1;
	}
	printf("fd = %d\n",fd);

	fd1 = open("2.txt",O_WRONLY | O_CREAT | O_TRUNC,0664);
	fd2 = open("3.txt",O_WRONLY | O_CREAT | O_APPEND,0664);
	
	printf("fd1 = %d\n",fd1);
	printf("fd2 = %d\n",fd2);

	close(fd1);
	fd3 = open("4.txt",O_WRONLY | O_CREAT | O_APPEND,0664);
	printf("fd3 = %d\n",fd3);

	return 0;
}

//创建的文件实际权限 = open的第三个参数 & ~umask
//0123  
//001 010 011 
//取反 110 101 100
//   & 110 110 100 
//     110 100 100 	
