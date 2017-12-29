#include<stdio.h>
#include<signal.h>

void func(int signo)
{
	printf("alarm !!!\n");
}

int main(int argc, const char *argv[])
{
	signal(SIGALRM,func);

	alarm(5);//只要定时时间到达默认会给当前进程发送SIGALRM信号

	printf("end\n");
	pause();
	return 0;
}
