#include<stdio.h>
#include<unistd.h>

int main(int argc, const char *argv[])
{
	pid_t pid;

	int fd[2];
	char buf[10];
	ssize_t n;
	pipe(fd);
	pid = fork();

	if(pid == -1)
	{

	}
	else if(pid == 0)//子进程读数据
	{
		sleep(5);
		close(fd[1]);	
		n = read(fd[0],buf,sizeof(buf));
		printf("n = %d,%s\n",n,buf);
	}
	else//父进程写数据 
	{
	//	sleep(5);
		close(fd[0]);
		write(fd[1],"hello",6);
		printf("end\n");
	}
	while(1);
	return 0;
}
