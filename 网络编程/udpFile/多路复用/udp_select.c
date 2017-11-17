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



int main(int argc, const char *argv[])
{
	int sockfd=socket(AF_INET,SOCK_DGRAM,0);
	
	if(sockfd<0)
	{
		perror("fail to socket");
		exit(1);
	
	}
	//bind   捆 扎  系
	//让  进程和端口号产生一个绑定关系  
	struct sockaddr_in my_addr,peer_addr;
	socklen_t len=sizeof(peer_addr);
	//memset   void * , 0, sizeof();
	bzero(&my_addr,sizeof(my_addr));
	
	my_addr.sin_family=AF_INET;
	my_addr.sin_port=htons(50000);
	// 网络需要大端数值  
	//
	//htons   host to network short  
	my_addr.sin_addr.s_addr=inet_addr("0.0.0.0");
	// 0.0.0.0 代表本机任意地址  自动适配
	//
	//
	//	my_addr.sin_addr.s_addr=inte_addr("172.22.1.250");
	//  字符串 ？ 大端数据么？
	//  inet_addr;    字符串 转换成  大端的整数数值 
	//
	int ret_bind=bind(sockfd,(struct sockaddr*)&my_addr,sizeof(my_addr));
	if(ret_bind<0)
	{
		
		perror("fail to bind");
		exit(1);
	
	
	}
	

	//  kernel  监视  io资源    
	//  0 ---》 fgets
	//  sockfd --》 recvfrom  
	//

	//select
	//select  阻塞  等待监视的文件描述符中出现  一个或者多个文件描述符   就绪 
	//
	//可能：
	//
	//0 就绪；或者 sockfd 就绪  或者  0， sockfd 都就绪
	//
	//1 nfds   文件描述符 集合中最大的那个文件描述符的数值  +1
	//类似  数组最大元素的索引 +1  得到这个数组的大小
	//
	//2 fd_set * readfds  读表  跟读资源相关的文件描述符要存放在这里
	//3 writefds 写表
	//4. exceptfds 异常表
	// 5 时间结构体的指针    设置超时的时间
	//
	// 返回值：  三个集合中就绪的文件描述的个数  
	// 不就绪的会被清除出去
	// void FD_CLR(int fd, fd_set *set);    删除掉某一个文件描述符
      // int  FD_ISSET(int fd, fd_set *set);  判断是否在表里  在返回1 不在返回0
     //  void FD_SET(int fd, fd_set *set);   设置每个文件描述符 加入到某张表
     //  void FD_ZERO(fd_set *set);       清空

	// 
	

	int maxfd=sockfd;
	fd_set readfds;
	struct timeval tv;
	int ret_select;


	char buf[128];


	while(1)
	{

		FD_ZERO(&readfds);

		FD_SET(0,&readfds);
		FD_SET(sockfd,&readfds);

		tv.tv_sec=5;
		tv.tv_usec=0;


		ret_select=select(maxfd+1,&readfds,NULL,NULL,&tv);
		//  select 在进行前将  关心的文件描述符  填充进去
		//  而return 的之后
		//  就绪的文件描述符  还在  而不就绪的会被清掉
		if(ret_select<0)
		{
			perror("fail to select");
			exit(1);
		
		
		}
		else if(ret_select==0)
		{
			printf("time out!!!\n");
			continue;


		}
		else
		{
			if(FD_ISSET(0,&readfds))
			{
				//fgets
				bzero(buf,sizeof(buf));
				fgets(buf,sizeof(buf),stdin);
				printf("fgets:%s\n",buf);
			
			
			
			}

			if(FD_ISSET(sockfd,&readfds))
			{
				//recvfrom
				bzero(buf,sizeof(buf));
				recvfrom(sockfd,buf,sizeof(buf),0,NULL,NULL);
				printf("recvfrom :%s\n",buf);
			
			
			}


		}
	
	
	
	}


	return 0;
}
