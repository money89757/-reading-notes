#include<stdio.h>
#include<signal.h>
void handler(int signo)
{
	printf("entry handler\n");
	if(signo == SIGINT)
		printf("my name is SIGINT\n");
	if(signo == SIGTSTP)
		printf("my name is SIGTSTP\n");
	if(signo == SIGSTOP)
		printf("SIGSTOP\n");
}

int main(int argc, const char *argv[])
{
//	signal(SIGINT,handler);//捕捉SIGINT信号——修改SIGINT信号的默认操作
//	signal(SIGTSTP,handler);
//	signal(SIGSTOP,handler);//SIGSTOP和SIGKILL不能被捕捉、忽略、阻塞
//	signal(SIGINT,SIG_IGN);//忽略信号时，不会唤醒pause函数
//	signal(SIGKILL,SIG_IGN);
	
	signal(SIGINT,SIG_DFL);//执行信号的默认操作

#if 0
	while(1)
	{
		printf("hello\n");
		sleep(1);
	}
#endif 

	printf("start\n");
	pause();//当进程没有收到信号则阻塞，收到信号后先运行中断函数(handler)，最后打印end，并且结束进程
	pause();
	printf("end\n");
	return 0;
}
