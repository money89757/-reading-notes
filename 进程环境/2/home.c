#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>

int main(int argc, const char *argv[])
{
//	int fd;
	int fd1,fd2;
	pid_t pid;
	off_t len;
	ssize_t n;
	char buf[10];
	if(argc < 3)
	{
		fprintf(stderr,"argv intput error\n");
		exit(EXIT_FAILURE);
	}


//	fd = open("1.txt",O_RDONLY);
	fd1 = open(argv[1],O_RDONLY);
	if(fd1 == -1)
	{

	}

	fd2 = open(argv[2],O_WRONLY | O_CREAT | O_TRUNC,0664);
	if(fd2 == -1)
	{

	}
	
	len = lseek(fd1,0,SEEK_END);
	len = len / 2;
	pid = fork();
	if(pid == -1)
	{

	}
	else if(pid == 0)
	{
#if 0
		fd = open("1.txt",O_RDONLY);//加了这一步，lseek返回值为10，说明父子进程使用不同的文件指针
		lseek(fd,5,SEEK_SET);
#endif
#if 1
		fd1 = open(argv[1],O_RDONLY);
		fd2 = open(argv[2],O_WRONLY);
		
		lseek(fd1,len,SEEK_SET);
		lseek(fd2,len,SEEK_SET);

		while((n = read(fd1,buf,sizeof(buf))) > 0)
		{
			write(fd2,buf,n);
		}
		close(fd1);
		close(fd2);
#endif
	}
	else 
	{
#if 0
		sleep(1);
		len = lseek(fd,10,SEEK_CUR);//返回值为15，说明父子进程使用同一个文件指针
		printf("%ld\n",len);
#endif
		lseek(fd1,0,SEEK_SET);

		int total = 0;
		while(1)
		{
			n = read(fd1,buf,sizeof(buf));
			total += n;

			if(total > len)
			{
				write(fd2,buf,sizeof(buf) - (total - len));
				break;
			}
			write(fd2,buf,n);
		}

		close(fd1);
		close(fd2);

		
	}
	return 0;
}
