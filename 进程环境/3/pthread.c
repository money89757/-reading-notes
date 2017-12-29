#include<stdio.h>
#include<pthread.h>

pthread_mutex_t mutex;
pthread_cond_t cond;
int buf[5] = {1,2,3,4,5};

void *fun1(void *arg)
{
	int i;
	//上锁
	pthread_mutex_lock(&mutex);
	for(i = 0;i < 5;i ++)
	{
		printf("%d\n",buf[i]);
		sleep(1);
	}
	//解锁
	pthread_mutex_unlock(&mutex);
	pthread_cond_signal(&cond);
}

void *fun3(void *arg)
{
	while(1)
	{
		printf("我是来打酱油的\n");
		sleep(1);
	}
}

void *fun2(void *arg)
{
	int i;

	//上锁
	pthread_mutex_lock(&mutex);
	pthread_cond_wait(&cond,&mutex);
	for(i = 4;i >= 0;i --)
	{
		printf("%d\n",buf[i]);
		sleep(1);
	}
	//解锁
	pthread_mutex_unlock(&mutex);
}

int main(int argc, const char *argv[])
{
	int ret;
	pthread_t tid1,tid2,tid3;
	
	ret = pthread_create(&tid1,NULL,fun1,NULL);
	ret = pthread_create(&tid2,NULL,fun2,NULL);
	ret = pthread_create(&tid3,NULL,fun3,NULL);

	//初始化锁
	pthread_mutex_init(&mutex,NULL);
	pthread_cond_init(&cond,NULL);
	while(1);
	return 0;
}
