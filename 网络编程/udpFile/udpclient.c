#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<strings.h>
#include<arpa/inet.h>
#include"udpmsg.h"
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>

int getfilename(const char *  path, int size ,char * filename);

int do_sendfile(int sockfd , struct  sockaddr_in *  pserver);

int main(int argc, const char *argv[])
{
	int sockfd;
	int nlen;
	int no;

	struct  sockaddr_in addrServer,addrRemote;

	addrServer.sin_family = AF_INET;
	addrServer.sin_port =  htons(49999);
	addrServer.sin_addr.s_addr  = inet_addr("0.0.0.0");

	bzero(&addrRemote , sizeof(addrRemote));
	
	nlen  =  sizeof(addrRemote);

	sockfd = socket(AF_INET,SOCK_DGRAM,0);
	
	if( sockfd < 0)
	{
		perror("Create  Socket");
		return -1;
	}
	
	int ncontinue =1;
	while(ncontinue)
	{

		printf("***************************\n");
		printf("* 1.SEND FILE     2.QUIT  *\n");
		printf("***************************\n");

		printf("choose(1~2):");

		no = getchar() -'0';
		while(getchar()!='\n');


		switch(no)
		{
			case 1:
				 do_sendfile(sockfd,  &addrServer);
				break;
					

			case 2:
				ncontinue =0;
				break;

			default:
				break;

		}


	}


	close(sockfd);
	return 0;
}

int do_sendfile(int sockfd  , struct sockaddr_in *  pserver)
{
	 char  path[256];
	 int  filefd;
	 long  npackcount;
	 long  nfilesize;
	 long  npackno =0;
	 int  nlen=0;

	 udpmsg_t  senddata , recvdata;

	 struct  sockaddr_in addrServer,addrRemote;

	bzero(&addrRemote , sizeof(addrRemote));

	nlen  =  sizeof(addrRemote);

	printf("\nplease input filename:");
	
	scanf("%s",path);

	while(getchar()!='\n');

	filefd  =  open(path, O_RDONLY);

	if( filefd < 0)
	{
		perror("open file fail");
		return  -1;
	}

	nfilesize  = file_len(filefd);

	npackcount  = nfilesize /  PACK_LEN;

	if (nfilesize %  PACK_LEN > 0)  npackcount ++;

	bzero(&senddata ,sizeof(udpmsg_t));

	getfilename(path, sizeof(path),senddata.data);
	
	senddata.type =  SEND_NAME;
	senddata.packno  = npackcount;
	senddata.crc = sum_check(senddata.data, PACK_LEN);

	sendto(sockfd, &senddata, sizeof(udpmsg_t),0,(struct sockaddr *) pserver, sizeof(struct sockaddr_in));

//	bzero(pdata , sizeof(udpmsg_t));
	int ncontinue =1;

	while(ncontinue)
	{
		bzero(&recvdata, sizeof(udpmsg_t));

		recvfrom(sockfd, &recvdata, sizeof(udpmsg_t),0, (struct sockaddr *) &addrRemote,&nlen);
		
		switch(recvdata.type)
		{
			case RECV_NAME_OK:
				 
				bzero(&senddata, sizeof(udpmsg_t));
				npackno=0;
				senddata.type  = SEND_DATA;
				senddata.packno = npackno;
				
				lseek(filefd, npackno * PACK_LEN , SEEK_SET);
			    	senddata.actlen = read(filefd, senddata.data, PACK_LEN);
				senddata.crc  =  sum_check( senddata.data,PACK_LEN);
				sendto(sockfd, &senddata, sizeof(udpmsg_t),0, (struct sockaddr *) &addrRemote,nlen);
				printf("RECV_NAME_OK=%ld\n", npackno);
				
				if(npackno >= npackcount -1)
				{
					ncontinue = 0;
					bzero(&senddata,sizeof(senddata));
					printf("send finish!\n");
				}

				break;

			case RECV_NAME_ERR:	
			case RECV_DATA_ERR:
				sendto(sockfd, &senddata, sizeof(udpmsg_t),0, (struct sockaddr *) &addrRemote,nlen);
				break;


			case RECV_DATA_OK:

				bzero(&senddata, sizeof(udpmsg_t));

				npackno = recvdata.packno +1;
				if( npackno >=npackcount-1)	
				{
					ncontinue =0;

					bzero(&senddata, sizeof(udpmsg_t));
					printf("send finish!\n");
				
				}
			


				senddata.type = SEND_DATA;
				lseek(filefd,npackno * PACK_LEN, SEEK_SET);
				senddata.packno = npackno;
				senddata.actlen=read(filefd, senddata.data, PACK_LEN);
				senddata.crc  =  sum_check( senddata.data,PACK_LEN);
				sendto(sockfd, &senddata, sizeof(udpmsg_t),0, (struct sockaddr *) &addrRemote,nlen);
				printf("RECV_DATA_OK=%ld\n",npackno);
		}

	}





	return 0;
}

int getfilename(const char *  path, int size ,char * filename)
{
	int i=0;
	int index=0;	

	if (size < 1) return -1;


	for(i=size-1; i>-1; i--)
	{
		if( path[i] =='/')  break;
	}

	for(++i; i<size; i++)
	{
		filename[index] = path[i];
		index++;
	}

	return  index;

}

