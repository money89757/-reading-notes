#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<semaphore.h>

//int flag = 0;
sem_t read_sem,write_sem;
void *read_file(void *arg)
{
	int fd;
	char buf[10];
	ssize_t n;
	fd = open("1.txt",O_RDONLY | O_CREAT,0664);
	if(fd == -1)
	{

	}

	while(1)
	{
//		while(!flag);
		sem_wait(&read_sem);
		n = read(fd,buf,sizeof(buf));
		write(1,buf,n);
		sem_post(&write_sem);
//		flag = 0;
	}
}

void *write_file(void *arg)
{
	char buf[10];
	ssize_t n;
	int fd;

	fd = open("1.txt",O_WRONLY | O_CREAT | O_TRUNC,0664);
	if(fd == -1)
	{

	}
	while(1)
	{
//		while(flag);
		sem_wait(&write_sem);
		n = read(0,buf,sizeof(buf));
		if(n == -1)
		{

		}
		if(strncmp(buf,"quit",4) == 0)
			break;

		write(fd,buf,n);
		sem_post(&read_sem);
//		flag = 1;
	}
	pthread_exit(NULL);
}

int main(int argc, const char *argv[])
{
	int ret;
	pthread_t tid1,tid2;
	ret = pthread_create(&tid1,NULL,read_file,NULL);	
	if(ret != 0)
	{

	}

	ret = pthread_create(&tid2,NULL,write_file,NULL);
	if(ret != 0)
	{
		
	}
	
	sem_init(&read_sem,0,0);
	sem_init(&write_sem,0,1);

//	while(1);
	pthread_join(tid2,NULL);
	return 0;
}
