#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main(int argc, const char *argv[])
{
	pid_t pid;

	char *p;

	p = malloc(10);
	
	scanf("%s",p);//hello

	pid = fork();
	
	if(pid == -1)
	{

	}
	else if(pid == 0)//代表子进程
	{
		sleep(1);//操作系统遇见延时函数会认为当前进程暂时不需要分配时间片
		printf("child:%s\n",p);
	}
	else //父进程 
	{
		*(p + 1) = 'z';
		printf("parent:%s\n",p);
	}
	return 0;
}
