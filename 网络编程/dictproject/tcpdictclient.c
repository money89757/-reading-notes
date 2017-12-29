#include<stdio.h>
#include <sys/types.h>	    
#include <sys/socket.h>
#include<netinet/in.h>
#include<strings.h>
#include<arpa/inet.h>
#include"tcpmsg.h"
#include<stdlib.h>
#include<string.h> 
int  do_register(int fd, MSG * pmsg);

int  do_login(int fd, MSG * pmsg);

char curUser[30];

int main(int argc, const char *argv[])
{
	int  listenfd;
	struct sockaddr_in  addrServer;
	MSG   msg;

	int no;
	addrServer.sin_family = AF_INET;
	addrServer.sin_port  =  htons(50000);

	if (argc<2)
		addrServer.sin_addr.s_addr  =  inet_addr("0.0.0.0");
	else
		addrServer.sin_addr.s_addr = inet_addr(argv[1]);

	listenfd  =  socket(AF_INET, SOCK_STREAM, 0);

	connect(listenfd, (struct sockaddr *)  &addrServer, sizeof(addrServer));

	while(1)
	{   
		
		printf("\n");		
		printf("****************************************\n");
		printf("*     1:register   2:login    3:quit   *\n");
		printf("****************************************\n");
		printf("please choose(1~3):");		

		no = getchar()-'0';

		while(getchar()!='\n');  //clear buffer of  stdin
		
		system("clear");
		
		switch(no)
		{
			case 1:
				printf("\n");
				do_register(listenfd, &msg);
				printf("\n");
				break;
			case 2:
				printf("\n");
				if (1==	do_login(listenfd, &msg))
				{
					do_next(listenfd,&msg);
				}
				printf("\n");
				break;
			
			case 3:
				close(listenfd);
				exit(0);
				break;
			default:
				break;
		
		
		}

		
	}

	close(listenfd);

	return 0;
}

int  do_register(int fd, MSG * pmsg)
{
	bzero(pmsg, sizeof(MSG));
	pmsg->type = R;	
	printf("Name:");
	scanf("%s",pmsg->name);
	printf("Password:");
	scanf("%s",pmsg->data);
	
	send(fd,pmsg, sizeof(MSG),0);
	
	bzero(pmsg, sizeof(MSG));
	recv(fd,pmsg,sizeof(MSG),0);

	printf("status: %s\n",pmsg->data);

	return  pmsg->name[0] -'0';




}

int  do_login(int fd, MSG * pmsg)
{
	bzero(pmsg, sizeof(MSG));
	pmsg->type = L;	
	printf("Name:");
	scanf("%s",pmsg->name);
	printf("Password:");
	scanf("%s",pmsg->data);
	
	send(fd,pmsg, sizeof(MSG),0);
	
	bzero(curUser,sizeof(curUser));

	strcpy(curUser, pmsg->name);

	bzero(pmsg, sizeof(MSG));
	recv(fd,pmsg,sizeof(MSG),0);

	printf("status: %s\n",pmsg->data);

	return  pmsg->name[0] -'0';
}

void replace(char *buf)
{
	int i=0;

	while(buf[i]!='\0')
	{
		switch(buf[i])
		{
			case '#':
				buf[i] =',';
				break;
			case '$':
				buf[i] = '\'';
				break;
			case '@':
				buf[i]='(';
				break;
			case '_':
				buf[i] = ')';
				break;

		}
		
		i++;
	}

}

int  do_query(int fd, MSG * pmsg)
{
	bzero(pmsg, sizeof(MSG));
	pmsg->type = Q;	
	printf("word:");
	scanf("%s",pmsg->data);

	strcpy(pmsg->name,curUser);

	send(fd,pmsg, sizeof(MSG),0);
	
	bzero(pmsg, sizeof(MSG));
	recv(fd,pmsg,sizeof(MSG),0);
	
	replace(pmsg->data);

	printf("DESC: %s\n",pmsg->data);

	return  pmsg->name[0] -'0';
}

int  do_history(int fd, MSG * pmsg)
{
	bzero(pmsg, sizeof(MSG));
	pmsg->type = H;	
	send(fd,pmsg, sizeof(MSG),0);
	
	while(1)
	{
		bzero(pmsg, sizeof(MSG));
		recv(fd,pmsg,sizeof(MSG),0);
		if (pmsg->data[0]==0)break;
		printf("%s\n",pmsg->data);
	}
	return  pmsg->name[0] -'0';
}

int do_next(int fd, MSG * pmsg)
{
	int no;

	while(1)
	{
		while(getchar()!='\n');  //clear buffer of  stdin
		printf("\n");		
		printf("****************************************\n");
		printf("* 1:query_word 2:history_record 3:quit *\n");
		printf("****************************************\n");
		printf("please choose(1~3):");		
		
		no = getchar()-'0';
	
		system("clear");
		switch(no)
		{
			case 1:
				do_query(fd,pmsg);
				break;
			case 2:
				do_history(fd,pmsg);
				break;
			case 3:
				while(getchar()!='\n');  //clear buffer of  stdin
				return 0;
				break;
			default:
				break;
		}

	}

	return 0;

}
