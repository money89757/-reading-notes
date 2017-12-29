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
	
	//捕捉信号

	key = ftok(".",'z');//注意:两端参数必须一致

	shmid = shmget(key,1024,0664 | IPC_CREAT | IPC_EXCL);
	if(shmid == -1)
	{
		if(errno == EEXIST)//后执行，共享内存区对方创建的
		{
			//读出对方PID
			//写自己PID到shm
			//给对方发送信号
			//只是负责打开共享内存
		
			//映射
		}
		else 
		{
			perror();
			exit(1);
		}
	}//先执行，当前进程创建并且打开了共享内存
	else
	{
		//写自己的PID 
		//阻塞
		//读出对方PID
		//映射 shmat
	}
		
	return 0;
}
