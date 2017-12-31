#include <stdio.h>
#include <sys/types.h>	       /* See NOTES */
#include <sys/socket.h>
#include <strings.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#define SA struct sockaddr

int main(int argc, const char *argv[])
{
	pid_t pid,ppid;
	//socket
	int sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0)
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


	int ret_recv;

//	signal(SIGCHLD,SIG_IGN);

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

		pid=fork();
		if(pid<0)
		{
			perror("fail to fork pid");
			exit(1);
		}
		else if (pid==0)
		{
			ppid=fork();
			if(ppid<0)
			{
				perror("fail to fork ppid");
				exit(1);
			
			}
			else if(ppid==0)// sun
			{
				close(sockfd);
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
						exit(0);


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
				else// zi
				{
				exit(0);
			
			
			}
		
		
		
		
		}
		else// fu
		{
			close(confd);
			wait(NULL);//子进程很快就退出了，可以用这个阻塞。父进程可以继续进行。
		
		
		}


	}

	return 0;
}


#if 0
	pid=fork();
		if(pid<0)
		{
			perror("fail to fork");
			exit(1);
		
		
		}

		else if (pid==0)
		{
			close(sockfd);
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
					exit(0);


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
		else
		{
		
			close(confd);
		
		}


#endif
