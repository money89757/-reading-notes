#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/shm.h>

#define READ 0  
#define WRITE 1 
union semun
{
	int val;
};

int main(int argc, const char *argv[])
{
	key_t key;
	int semid,shmid;
	union semun myun;
	char *p;
	struct sembuf sem;
	if((key = ftok(".",'a')) < 0)
	{
		perror("fail to ftok");
		return -1;
	}
	if((semid = semget(key,2,IPC_CREAT | IPC_EXCL| 0666)) < 0)
	{
		if(errno == EEXIST)
		{
			semid = semget(key,2,0666); 
		}
		else
		{
			perror("fail to semget");
			return -1;
		}
	}
	else
	{
		myun.val = 0;//0值具体代表的是某种资源的个数
		semctl(semid,READ,SETVAL,myun); //初始化资源个数,相当于sem_init()

		myun.val = 1;
		semctl(semid,WRITE,SETVAL,myun);//sem_t a  sem_init(&a,0,1);
	}

	if((shmid = shmget(key,1024,IPC_CREAT | 0666)) < 0)//只是帮助进程完成通信的
	{
		perror("fail to shmget");
		return -1;
	}
	

	p = (char *)shmat(shmid,NULL,0);

	while(1)
	{
		//p(READ)
		sem.sem_num = READ;
		sem.sem_op = -1;
		sem.sem_flg = 0;
		semop(semid,&sem,1);
		//以上操作相当于执行sem_wait();

		printf("read from shm:%s",p);
		if(strncmp(p,"quit",4) == 0)
			break;

		sem.sem_num = WRITE;
		sem.sem_op = 1;
		sem.sem_flg = 0; 
		semop(semid,&sem,1);
		//以上操作相当于执行sem_post();
	}
	shmdt(p);
	shmctl(shmid,IPC_RMID,NULL);
	semctl(semid,2,IPC_RMID);

	return 0;
}
