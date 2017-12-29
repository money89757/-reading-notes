#include <stdio.h>
//#include <sys/types.h>	       /* See NOTES */
#include <sys/socket.h>
#include <strings.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
// #include <sys/select.h>
 #include <sys/epoll.h>
#include <errno.h>


#define SA struct sockaddr

int main(int argc, const char *argv[])
{
	//socket
	int confd;
	int sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0)
	{
		perror("fail to socket");
		exit(1);
	
	
	}
	//man 7 ip
	//  SOCK_STREAM  流式套接字
	
	int opt=1;
	setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
	// SO_REUSEADDR  督促内核重新去监测一下 端口号是否还是被占用
	// 如果不是就可以再次bind  



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

	struct epoll_event ev,events[50];
	int nfds;


	int epollfd=epoll_create(50);
	// size  在新版本内核中只是一个hint 提示 不够的时候
	// epoll自己增加内核中结构体的个数
	//
	// 但是 在ubuntu 12.04版本中 内核 查询命令  uname -a
	// 3.13  较旧    所以此特性 不成立
	// 建议 设置为 监视的文件描述符的最大个数  
	if(epollfd<0)
	{
		perror("fail to epoll create");
		exit(1);
	}


	//添加监视对象
	ev.events=EPOLLIN;// 监视读资源
	ev.data.fd=0;
	if(epoll_ctl(epollfd,EPOLL_CTL_ADD,0,&ev)==-1)
	{
		perror("fail to epoll add 0");
		exit(1);
	
	
	
	}

	ev.events=EPOLLIN;// 监视读资源
	ev.data.fd=sockfd;
	if(epoll_ctl(epollfd,EPOLL_CTL_ADD,sockfd,&ev)==-1)
	{
		perror("fail to epoll add sockfd");
		exit(1);
	
	
	
	}

	while(1)
	{
		nfds=epoll_wait(epollfd,events,50,-1);
		//nfds   就绪的文件描述符的个数
		//
		//epollfd   epoll的fd  
		//
		//events  返回就绪结果的数组名
		//50  该数组最大元素个数
		//-1  代表不使用超时  

		//  就绪了nfds个文件描述符  按照从0 到nfds-1 的数组顺序放在events[] 
		//
		if(nfds==-1)
		{
			perror("fail to epoll wait");
			exit(1);

		
		
		}
		

		int i;
		for(i=0;i<nfds;i++)  //此时遍历的每一个元素都是就绪的  不同于 select
		{
			if(events[i].data.fd==0)  //0
			{
				bzero(buf,sizeof(buf));
				fgets(buf,sizeof(buf),stdin);
				printf("fgets:%s\n",buf);


			}
			else if(events[i].data.fd==sockfd) //sockfd
			{
				confd=accept(sockfd,(SA*)&peer_addr,&len);
				if(confd<0)
				{
					perror("fail to accept");
					exit(1);


				}

				ev.events=EPOLLIN;// 监视读资源
				ev.data.fd=confd;
				if(epoll_ctl(epollfd,EPOLL_CTL_ADD,confd,&ev)==-1)
				{
					perror("fail to epoll add confd");
					exit(1);



				}





			}
			else //connfd
			{

				bzero(buf,sizeof(buf));
				int ret_recv=recv(events[i].data.fd,buf,sizeof(buf),0);
				if(ret_recv<0)
				{
					perror("fail to recv");
					printf("errno = %d\n",errno);
					exit(1);


				}
				else if(ret_recv==0)
				{
					printf("peer is shutdown!!\n");

					if(epoll_ctl(epollfd,EPOLL_CTL_DEL,events[i].data.fd,NULL)==-1)
					{
						perror("fail to epoll del confd");
						exit(1);



					}



					close(events[i].data.fd);



				}
				else
				{
					printf("recv:%s\n",buf);

					send(events[i].data.fd,buf,sizeof(buf),0);


				}



			}


		}

	}







	return 0;
}




#if 0


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
		//
		//  //confd   们
		//     recv
		//     <0
		//     ==0   -->    close(i);   --> i==maxfd   -->  找次最大值
		//     >0
		//
		//  }
		//
		//
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






#endif
