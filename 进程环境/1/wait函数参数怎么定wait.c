#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main(int argc, const char *argv[])
{
	int status;
	pid_t pid;
	pid = fork();

	if(pid == -1)
	{

	}
	else if(pid == 0)//代表子进程
	{
		printf("PID:%d\n",getpid());
		exit(3);
	}
	else //父进程 
	{
		printf("父进程\n");
	//	wait(NULL);
		wait(&status);
		printf("%#x\n",status);
		printf("end\n");
	}

	while(1);
	return 0;
}
