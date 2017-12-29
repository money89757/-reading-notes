#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<semaphore.h>

pthread_mutex_t mutex;
pthread_cond_t cond;

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
		pthread_mutex_lock(&mutex);
		pthread_cond_wait(&cond,&mutex);//解锁，阻塞 .... 接收到cond条件唤醒，上锁 
		n = read(fd,buf,sizeof(buf));
		write(1,buf,n);
		pthread_mutex_unlock(&mutex);
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
		usleep(500);
		pthread_mutex_lock(&mutex);
		n = read(0,buf,sizeof(buf));
		if(n == -1)
		{

		}
		if(strncmp(buf,"quit",4) == 0)
			break;

		write(fd,buf,n);
		pthread_mutex_unlock(&mutex);
		pthread_cond_signal(&cond);//获取了产生的条件，然后发送给wait函数
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
	
	pthread_mutex_init(&mutex,NULL);
	pthread_cond_init(&cond,NULL);

	pthread_join(tid2,NULL);
	return 0;
}
