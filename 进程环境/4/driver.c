#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>

pid_t pid;
void sailer_handler(int signo)
{
	if(signo == SIGINT)
		kill(getppid(),SIGUSR1);
	if(signo == SIGQUIT)
		kill(getppid(),SIGUSR2);
	if(signo == SIGUSR1)
	{
		printf("get off the bus\n");
		kill(getppid(),SIGKILL);
		exit(0);
	}
}

void driver_handler(int signo)
{
	if(signo == SIGUSR1)
		printf("lets gogogo\n");

	if(signo == SIGUSR2)
		printf("stop the bus\n");

	if(signo == SIGTSTP)
		kill(pid,SIGUSR1);
}

int main(int argc, const char *argv[])
{

	pid = fork();

	if(pid == -1)
	{

	}
	else if(pid == 0)//售票员
	{
		signal(SIGINT,sailer_handler);

		signal(SIGQUIT,sailer_handler);

		signal(SIGTSTP,SIG_IGN);
		signal(SIGUSR1,sailer_handler);
	}
	else 
	{
		signal(SIGINT,SIG_IGN);
		signal(SIGUSR1,driver_handler);

		signal(SIGQUIT,SIG_IGN);
		signal(SIGUSR2,driver_handler);

		signal(SIGTSTP,driver_handler);
	}
	while(1);
	return 0;
}
