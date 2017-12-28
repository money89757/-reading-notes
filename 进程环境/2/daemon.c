#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<time.h>

int main(int argc, const char *argv[])
{
	pid_t pid;

	pid = fork();

	if(pid == -1)
	{

	}
	else if(pid == 0)
	{
		int fd_max;
		int fd;
		time_t t;
		struct tm *ptm;
		setsid();
		umask(0);
		chdir("/home/linux/");
		
		fd_max = getdtablesize();
		for(fd = 0 ;fd < fd_max;fd ++)
		{
			close(fd);
		}
		
		FILE *fp;

		fp = fopen("log","w");
		if(fp == NULL)
		{
			perror("fopen");
			exit(EXIT_FAILURE);
		}
		while(1)
		{
			//具体的守护进程实现的功能
			t = time(NULL);
			ptm = localtime(&t);

			fprintf(fp,"%d-%d\n",ptm->tm_year + 1900,ptm->tm_mon + 1);
			fflush(fp);
			sleep(1);
		}


	}
	else 
	{
		exit(EXIT_SUCCESS);
	}
	return 0;
}

