#include<stdio.h>
#include<stdlib.h>
#include<signal.h>

int main(int argc, const char *argv[])
{
	pid_t pid,pid1;
#if 0
	while(1)
	{
		pid = fork();
		if(pid == -1)
		{

		}
		else if(pid == 0)
		{
			//无限循环执行操作
			//某些条件成立时退出
			pid1 = fork();

			if(pid1 == -1)
			{

			}
			else if(pid1 == 0)
			{
				//孤儿进程
				//如果当前进程退出，则由init回收资源
			}
			else 
			{
				exit(0);	
			}
		}
		else 
		{
			wait(NULL);	
		}
	}
#endif 
	//僵尸进程:子进程退出，父进程没有回收资源；还因为父进程接收到了SIGCHLD信号
	//子进程退出时，如果不给父进程发送SIGCHLD信号，那么子进程的资源回收同样会交给init来做
	signal(SIGCHLD,SIG_IGN);
	while(1)
	{
		pid = fork();

		if(pid == -1)
		{}
		else if(pid == 0)
		{
			exit(0);
		}
		else 
		{
			
		}
	}

	return 0;
}
