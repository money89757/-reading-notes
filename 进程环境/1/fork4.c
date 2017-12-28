#include<stdio.h>
#include<unistd.h>

int main(int argc, const char *argv[])
{
	pid_t pid1,pid2;

	pid1 = fork();

	if(pid1 == -1)
	{

	}
	else if(pid1 == 0)//子进程1
	{
		printf("child1:PID:%d,PPID:%d\n",getpid(),getppid());
	}
	else
	{
		pid2 = fork();
		if(pid2 == -1)
		{
		}
		else if(pid2 == 0)//子进程2
		{
			printf("child2:PID:%d,PPID:%d\n",getpid(),getppid());
		}
		else //真正的父进程
		{
			printf("parent:PID:%d\n",getpid());
		}
	}
	while(1);
	return 0;
}
