#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>

int main(int argc, const char *argv[])
{
	key_t key;
	int shmid;
	
	char *p;

	key = ftok(".",'z');

	shmid = shmget(key,1024,0664 | IPC_CREAT);
	if(shmid == -1)
	{
		perror("shmget");
		return -1;
	}

	p = shmat(shmid,NULL,0);
	if(p == (void *)-1)
	{
		perror("shmat");
		return -1;
	}

	scanf("%s",p);//从终端读取数据直接写入共享内存区
	return 0;
}
