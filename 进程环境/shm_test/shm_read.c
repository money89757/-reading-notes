#include<stdio.h>
#include<sys/types.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<signal.h>
#include<errno.h>
#include<string.h>

typedef struct 
{
	pid_t pid;
	char buf[64];
}SHM;

void func(int sig)
{

}

int main(int argc, const char *argv[])
{
	key_t key;
	int shmid;
	
	pid_t peerpid;

	SHM *p;
	
	signal(SIGUSR1,func);//为了唤醒43行的pause函数

	key = ftok(".",'a');

	shmid = shmget(key,sizeof(SHM),0664 | IPC_CREAT | IPC_EXCL);
	if(shmid == -1)
	{
		if(errno == EEXIST)//后执行
		{

		}
		else 
		{
			perror("shmget");
			return -1;
		}
	}
	else //先执行 
	{
		p = shmat(shmid,NULL,0);

		p->pid = getpid();//将当前进程号存放到共享内存区
		pause();
		peerpid = p->pid;//获取了write.c的进程号
	}

	while(1)
	{
		pause();

		if(strncmp(p->buf,"quit",4) == 0)
			break;

		printf("read from shm_write.c %s\n",p->buf);
		kill(peerpid,SIGUSR1);//发送信号给write.c
	}

	shmctl(shmid,IPC_RMID,NULL);
	return 0;
}
