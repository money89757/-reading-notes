#include<stdio.h>
#include<unistd.h>


int main(int argc, const char *argv[])
{
	pid_t pid;
	pid = fork();

	if(pid == -1)
	{

	}
	else if(pid == 0)//代表子进程
	{
		printf("child:pid:%d,PID:%d,PPID:%d\n",pid,getpid(),getppid());
	}
	else //父进程 
	{
		printf("parent:pid:%d,PID:%d,PPID:%d\n",pid,getpid(),getppid());
	}
	while(1);
	return 0;
}
