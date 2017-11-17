#include <stdio.h>
#include <sys/types.h>	       /* See NOTES */
#include <sys/socket.h>
#include <strings.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
 #include <sys/select.h>


#define SA struct sockaddr

int main(int argc, const char *argv[])
{
	//socket
	int confd;
	int sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0) 】
	{
		perror("fail to socket");
		exit(1);
	
	
	}
	//man 7 ip
	//  SOCK_STREAM  流式套接字
	//
	//bind
	struct sockaddr_in my_addr,peer_addr;
	bzero(&my_addr,sizeof(my_addr));
	my_addr.sin_family=AF_INET;
	my_addr.sin_port=htons(50000);
	my_addr.sin_addr.s_addr=inet_addr("0.0.0.0");

	int ret_bind=bind(sockfd,(SA *)&my_addr,sizeof(my_addr));
	if(ret_bind<0)
	{
		perror("fail to bind");
		exit(1);
	
	
	
	}

	char buf[128];

	socklen_t len=sizeof(peer_addr);

	listen(sockfd,5);

	//listen
	//该函数将设置 sockfd 变为被动模式：即将用于接收 到来的连接请求
	//
	//backlog
	//定义了一个等待队列的最大长度，当队列满，而有链接到达的时候，对方会收到一个错误：ECONNREFUSED
	//accept
	// 从队列中抽取第一个等待连接 创造一个新的连接套接字，并返回文件描述符


	fd_set global,current;
	// global  增加或者删除要监视的文件描述符   在global 进行操作
	// 过筛子 current 每一次select 开始之前 重新用  global 给current 赋值
	FD_ZERO(&global);
	FD_SET(0,&global);
	FD_SET(sockfd,&global);


	int maxfd=sockfd;


	int ret_select;
	struct timeval tv;
	while(1)
	{
		FD_ZERO(&current);
		current=global;
		//memcpy(&current,&global,sizeof(fd_set));
		tv.tv_sec=5;
		tv.tv_usec=0;

		ret_select=	select(maxfd+1,&current,NULL,NULL,&tv);
		if(ret_select<0)
		{
			perror("fail to select");
			exit(1);
		
		}
		else if(ret_select==0)
		{
			printf("time out!\n");
			continue;

		}

		if(FD_ISSET(0,&current))
		{
			//fgets
			bzero(buf,sizeof(buf));
			fgets(buf,sizeof(buf),stdin);
			printf("fgets:%s\n",buf);
		
		}
		if(FD_ISSET(sockfd,&current))
		{
			//accept
			//
			//
			//  FD_SET(connfd,&global)
			//
			//  if(maxfd<confd);
			confd=accept(sockfd,(SA*)&peer_addr,&len);
			if(confd<0)
			{
				perror("fail to accept");
				exit(1);
		
			}
			printf("peer %sis connected,confd is %d\n",inet_ntoa(peer_addr.sin_addr),confd);

			FD_SET(confd,&global);
			if(maxfd<confd)
			{
				maxfd=confd;
			
			}
		
		}
		//for(i=3;i<=maxfd;i++)
		//{
		//  if(FD_ISSET(i,&current)
		//  {
	//
		//  //confd   们
		//     recv
		//     <0
		//     ==0   -->    close(i);   --> i==maxfd   -->  找次最大值
		//     >0
		//
		//  }
//
		//}
		int i;
		for(i=3;i<=maxfd;i++)
		{
			if(FD_ISSET(i,&current))
			{
				if(i==sockfd)
					continue;
			
				bzero(buf,sizeof(buf));
				int ret_recv=recv(i,buf,sizeof(buf),0);
				if(ret_recv<0)
				{
					perror("fail to recv");
					exit(1);
				
				}
				else if(ret_recv==0)
				{
					printf("peer is shutdown!!\n");
					close(i);
					FD_CLR(i,&global);
					if(maxfd==i)
					{
						//find next maxfd;
						while(--maxfd)
						{
							if(FD_ISSET(maxfd,&global))
									break;		
						}
					}
				}
				else
				{
					printf("recv:%s\n",buf);

					send(i,buf,sizeof(buf),0);
		
				}
	
			}
	
		}

	}
	return 0;
}

#if 0

	int ret_recv;

	while(1)
	{
		int confd=accept(sockfd,(SA*)&peer_addr,&len);
		if(confd<0)
		{
			perror("fail to accept");
			exit(1);


		}
		//
		printf("ip:%s,port:%d is connected\n",inet_ntoa(peer_addr.sin_addr),ntohs(peer_addr.sin_port));

		while(1)
		{
			bzero(buf,sizeof(buf));

			//recv
			ret_recv=recv(confd,buf,sizeof(buf),0);
			if(ret_recv<0)
			{
				perror("fail to recv");
				exit(1);


			}
			else if (ret_recv==0)
			{
				printf("peer is shutdown\n");
				close(confd);
				break;


			}
			else
			{

				printf("%s\n",buf);
				//send
				send(confd,buf,strlen(buf),0);
				//
				//close
			}
		}
	}


#endif
