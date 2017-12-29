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

void func(int signo)
{

}

int main(int argc, const char *argv[])
{
	key_t key;
	int shmid;
	
	pid_t peerpid;

	SHM *p;
	
	signal(SIGUSR1,func);

	key = ftok(".",'a');

	shmid = shmget(key,sizeof(SHM),0664 | IPC_CREAT | IPC_EXCL);
	if(shmid == -1)
	{
		if(errno == EEXIST)//后执行
		{
			shmid = shmget(key,sizeof(SHM),0664);
			p = shmat(shmid,NULL,0);
			peerpid = p->pid;//从共享内存中拿出对方的PID存放到peerpid变量中	
			p->pid = getpid();
			kill(peerpid,SIGUSR1);
		}
		else 
		{
			perror("shmget");
			return -1;
		}
	}
	else //先执行 
	{
	}

	while(1)
	{
		fgets(p->buf,sizeof(p->buf),stdin);//从终端读取数据
		kill(peerpid,SIGUSR1);//一旦发送成功，read.c的第二个pause函数被唤醒

		if(strncmp(p->buf,"quit",4) == 0)
			break;

		pause();//防止循环发送
	}
	return 0;
}
