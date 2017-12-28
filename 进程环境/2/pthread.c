#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>

int global_data = 100;

void *fun1(void *arg)
{
	while(1)
	{
		printf("fun1:global_data = %d\n",global_data);
		sleep(1);
	}
}

void *fun2(void *arg)//接收了a的地址
{
	int i;
	int value = *(int *)arg;
	static int x = 3;
	for(i = 0;i < 5;i ++)
	{
		printf("fun2:global_data = %d,value = %d\n",global_data,value);
		sleep(1);
	}
	pthread_exit((void *)&x);
}

int main(int argc, const char *argv[])
{
	int ret;
	pthread_t tid1,tid2;
	int a = 10;
	int *p;

	ret = pthread_create(&tid1,NULL,fun1,NULL);
	if(ret != 0)
	{
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}

	ret = pthread_create(&tid2,NULL,fun2,(void *)&a);
	if(ret != 0)
	{

	}

//	while(1);
	pthread_join(tid2,(void **)&p);//pthread_exit的参数传递给了p指针

	printf("%d\n",*p);
	printf("end\n");
	return 0;
}
