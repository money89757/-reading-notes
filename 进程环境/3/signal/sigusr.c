#include<stdio.h>
#include<signal.h>
void func(int sig)
{
	if(sig == SIGUSR1)
		printf("my name is SIGUSR1\n");
}

int main(int argc, const char *argv[])
{
	signal(SIGUSR1,func);//SIGUSR1叫做用户自定义信号，产生的条件由用户自己指定

	char buf[10];
	gets(buf);

	if(strncmp(buf,"start",5) == 0)
		kill(getpid(),SIGUSR1);

	return 0;
}
