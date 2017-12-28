#include<stdio.h>
#include<unistd.h>


int main(int argc, const char *argv[])
{
	pid_t pid;
	int a = 10;
	int b;
	pid = fork();

	if(pid == -1)
	{

	}
	else if(pid == 0)//代表子进程
	{
		a ++;
		printf("child:a = %d\n",a);
		printf("child:b = %d\n",b);
		printf("child:&a = %p\n",&a);
	}
	else //父进程 
	{
		a ++;
		b = 100;
	
		printf("parent:a = %d\n",a);
		printf("parent:b = %d\n",b);
		printf("parent:&a = %p\n",&a);
	}
	return 0;
}
