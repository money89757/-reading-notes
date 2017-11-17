#ifndef  _UDPFILE_H_
#define  _UDPFILE_H_

#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/socket.h>

#define  PACK_LEN (1024 * 8) //8K
#define  PATH_MAX 256
#define  TASK_MAX 10

#define  SEND_NAME 1
#define  SEND_DATA 2
#define  SEND_END  3

#define  RECV_NAME_OK   4
#define  RECV_NAME_ERR  5

#define  RECV_DATA_OK   6
#define  RECV_DATA_ERR  7

typedef  struct
{
	int  type;    //command type 
	long packno;  // packet  No.
	char data [PACK_LEN]; // data
	int  actlen;
	int  crc;    // crc

} udpmsg_t;

typedef  struct
{
	unsigned int  useraddr;   //ip
	unsigned short userport;  //port
	int filefd;  
	int used;  
	long npackcount;

} udptask_t;


void init_task(udptask_t * task)
{
	task->useraddr=0;
	task->userport=0;
	task->filefd =-1;
	task->used =0;

}

void init_tasks(udptask_t * tasks, int size)
{
	int i;
	for(i=0; i<size; i++) init_task(&(tasks[i]));
}


int  is_tasks(udptask_t * tasks, int size, unsigned int  addr, unsigned short  port)
{
	int i =0;

	for(i=0; i<size; i++)
	{
		if(tasks[i].used==1 && tasks[i].useraddr ==  addr && tasks[i].userport == port)
			return i;
	}

	return  -1;
}


int add_tasks(udptask_t * tasks , int size, unsigned int addr, unsigned short port)
{	int i=0;

	for(i=0; i<size; i++)
	{
		if(tasks[i].used ==0)
		{
			tasks[i].used=1;
			tasks[i].useraddr = addr;
			tasks[i].userport = port;
			return  i;
		}

	}

	return -1;

}



int sum_check(char * buf,   int  size)
{
	int i;
	int sum=0;

	
	for(i=0; i<size;i++)
		sum+=buf[i];

	if(sum>0xFF) 
	{
		sum+=1;
		sum=~sum;
	}

	return  sum&=0XFF;

}

long  file_len(int fd)
{ 
	long  nlen;

	nlen =lseek(fd ,0L, SEEK_END);	


	lseek(fd,0L,SEEK_SET);

	return  nlen;	

}





#endif

