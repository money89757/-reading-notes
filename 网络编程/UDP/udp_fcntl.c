#include <stdio.h>
#include <sys/types.h>	       /* See NOTES */
#include <sys/socket.h>
#include <strings.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>



int main(int argc, const char *argv[])
{
	int sockfd=socket(AF_INET,SOCK_DGRAM,0);
	//socket
	//man socket
	//函数实现 创建端点  返回  文件描述符
	//domain  作用域  AF_INET--> ipv4  --> man 7 IP
	//
	//udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
	//
	//SOCK_DGRAM  -->udp   数据类型
	//SOCK_STREAM  --> tcp  专用数据类型
	//  vi  /etc/protocols  
	
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



	//fcntl   F_SETFL   ---> O_NONBLOCK
	//
	//O_NONBLOCK   0 0004000
	//
	//
	//first F_GETFL     file status flag  -->  |= O_NONBLOCK second F_SETFL
	
	int flag1=fcntl(sockfd,F_GETFL);

	flag1|=O_NONBLOCK;

	fcntl(sockfd,F_SETFL,flag1);

	int flag2=fcntl(0,F_GETFL);

	flag2|=O_NONBLOCK;
	fcntl(0,F_SETFL,flag2);

#if 0
	flag1&=~O_NONBLOCK;
#endif






	char buf[128];
	int ret_recvfrom;
	while(1)
	{
		sleep(1);

		bzero(buf,sizeof(buf));
		bzero(&peer_addr,len);


		if(fgets(buf,sizeof(buf),stdin))
		{
			printf("fgets:%s\n",buf);
		
		
		
		}


		ret_recvfrom=recvfrom(sockfd,buf,sizeof(buf),0,(struct sockaddr*)(&peer_addr),&len);

		if(ret_recvfrom<0)
		{
			if(errno!=EAGAIN)
			{
				perror("fail to recv");
				printf("errno = %d\n",errno);
				return 0;
			}
			printf("hehe no data\n");
			
		
		}
		else 
		{

		printf("ip:%sport:%d buf:%s\n",inet_ntoa(peer_addr.sin_addr),ntohs(peer_addr.sin_port),buf);

		//  ntohs  network to host short
		//
		//  inet_ntoa   network to ascii
		//
		//
	
		sendto(sockfd,buf,strlen(buf),0,(struct sockaddr*)&peer_addr,len);
		}
	
	
	}



	return 0;
}
