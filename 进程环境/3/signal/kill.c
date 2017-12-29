#include<stdio.h>
#include<signal.h>

int main(int argc, const char *argv[])
{
#if 0
	kill(getpid(),SIGKILL);	
	while(1);
#endif

	pid_t pid;

	pid = fork();

	if(pid == -1)
	{}

	else if(pid == 0)
	{
		kill(getppid(),SIGKILL);//这里正在执行时，并不是直接杀死了父进程，仅仅是给父进程发送了一个信号而已	
		while(1);
	}
	else 
	{
		printf("parent\n");
		while(1);
	}

	return 0;
}
