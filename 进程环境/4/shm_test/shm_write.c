#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>

typedef struct 
{
	pid_t pid;
	char buf[64];
}shm_t;

int main(int argc, const char *argv[])
{
	key_t key;
	int shmid;

	key = ftok(".",'z');//注意:两端参数必须一致

	sh = shmget(key,1024,0664 | IPC_CREAT | IPC_EXCL);
	if(shmid == -1)
	{
		if(errno == EEXIST)//后执行，共享内存区对方创建的
		{

		}
		else 
		{
			perror();
			exit(1);
		}
	}//先执行，当前进程创建并且打开了共享内存
	{

	}
		
	return 0;
}
