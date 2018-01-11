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
		myun.val = 0;
		semctl(semid,READ,SETVAL,myun);

		myun.val = 1;
		semctl(semid,WRITE,SETVAL,myun);
	}

	if((shmid = shmget(key,1024,IPC_CREAT | 0666)) < 0)
	{
		perror("fail to shmget");
		return -1;
	}
	
	p = (char *)shmat(shmid,NULL,0);

	while(1)
	{
		//p(WRITE)
		sem.sem_num = WRITE;
		sem.sem_op = -1;
		sem.sem_flg = 0;
		semop(semid,&sem,1);
		
		printf(">");
		fgets(p,1024,stdin);
		
		sem.sem_num = READ;
		sem.sem_op = 1;
		sem.sem_flg = 0;
		semop(semid,&sem,1);

		if(strncmp(p,"quit",4) == 0)break;
	}
	shmdt(p);

	return 0;
}
