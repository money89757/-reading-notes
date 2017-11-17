#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<strings.h>
#include"udpmsg.h"
#include<arpa/inet.h>
#include<stdlib.h>
int main(int argc, const char *argv[])
{
	int sockfd;
	udpmsg_t senddata ,recvdata;
	
	udptask_t  tasks[TASK_MAX];
	int nlen;
//	int filefd;
	int index;
	char path[PATH_MAX];
	
//	long npackcount;

	struct  sockaddr_in  addrServer,addrClient;	

	init_tasks(tasks, TASK_MAX);

	addrServer.sin_family = AF_INET;
	addrServer.sin_port =  htons(49999);
	addrServer.sin_addr.s_addr  = inet_addr("0.0.0.0");

	nlen =  sizeof(addrClient);
	bzero(&addrClient , sizeof(addrClient));


	sockfd = socket(AF_INET,SOCK_DGRAM,0);
	
	if( sockfd < 0)
	{
		perror("Create  Socket");
		return -1;
	}

	int nRet  = bind(sockfd,(struct sockaddr *) &addrServer, sizeof(addrServer));

	if(nRet<0)
	{
		perror("bind Fail");
		return  -1;
	
	}

	while(1)
	{


		bzero(&addrClient , sizeof(addrClient));
		
		bzero(&recvdata,sizeof(recvdata));
		
		recvfrom(sockfd, &recvdata, sizeof(recvdata),0, (struct sockaddr*)  &addrClient, &nlen);	

		index =  is_tasks(tasks,TASK_MAX, addrClient.sin_addr.s_addr, addrClient.sin_port);

				
		if(index <0) 
			index =  add_tasks(tasks,TASK_MAX, addrClient.sin_addr.s_addr, addrClient.sin_port);

		
		if(index <0) //任务已满
		{
			printf("task full!\n");
			continue;
		}

		switch(recvdata.type)
		{
		
		case SEND_NAME:
			
		
			bzero(&senddata,sizeof(senddata));
			
			senddata.type  = RECV_NAME_ERR;

			if (sum_check(recvdata.data , PACK_LEN) ==  recvdata.crc)
			 {

				 bzero(path, sizeof(path));
				 sprintf(path, "./upload/%s" ,  recvdata.data);
				
				tasks[index].filefd  = open(path, O_RDWR|O_CREAT|O_TRUNC, 0666);

				if (tasks[index].filefd <0)  
				{
					perror("open to fail");
					return  -1;  //
				}

				senddata.type  = RECV_NAME_OK;
				tasks[index].npackcount =  recvdata.packno;
				tasks[index].used =1;
				printf("fileName=%s,packcount=%ld\n",recvdata.data,recvdata.packno+1);


			 }

			else
			{
				senddata.type=RECV_NAME_ERR;
			}

			printf("SEND_NAME=%s,packcount%ld\n",recvdata.data, recvdata.packno);

			sendto(sockfd, &senddata, sizeof(senddata),0, (struct sockaddr*)  &addrClient, nlen);	
			
			break;
		case SEND_DATA:

			bzero(&senddata,sizeof(senddata));
			
			system("clear");

			printf("npackcount=%ld, npackno=%ld\n",tasks[index].npackcount, recvdata.packno+1);
			
			senddata.type  = RECV_DATA_ERR;

			if (sum_check(recvdata.data , PACK_LEN) ==  recvdata.crc)
			 {
				write(tasks[index].filefd , recvdata.data, recvdata.actlen);
				
				if (tasks[index].npackcount <= recvdata.packno+1)
				{
					close(tasks[index].filefd);
					tasks[index].used=0;
					tasks[index].npackcount=0;
					printf("recv file finish!\n");
					break;
				}
				senddata.packno  = recvdata.packno;
				senddata.type  = RECV_DATA_OK;
			 	
			 }
			else
			{
				senddata.type =  RECV_DATA_ERR;
			
			}

			sendto(sockfd, &senddata, sizeof(senddata),0, (struct sockaddr*)  &addrClient, nlen);	


			break;
		
		
		}


		
	}


	close(sockfd);
	return 0;
}
