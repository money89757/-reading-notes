#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>

int main(int argc, const char *argv[])
{
	key_t key;
	int shmid;
	
	char *p;

	key = ftok(".",'z');//注意:两端参数必须一致

	shmid = shmget(key,1024,0664 | IPC_CREAT);
	if(shmid == -1)
	{
		perror("shmget");
		return -1;
	}

	p = shmat(shmid,NULL,0);
	if(p == (void *)-1)	{
		perror("shmat");
		return -1;
	}

	printf("%s\n",p);//将共享内存中的数据打印到终端
	
	shmctl(shmid,IPC_RMID,NULL);
	return 0;
}
