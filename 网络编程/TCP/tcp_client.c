#include <stdio.h>
#include <sys/types.h>	       /* See NOTES */
#include <sys/socket.h>
#include <strings.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define SA struct sockaddr

int main(int argc, const char *argv[])
{



	if(argc!=3)
	{
		printf("please input ip and port!\n");
		exit(1);

	}

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
	my_addr.sin_port=htons(60000);
	my_addr.sin_addr.s_addr=inet_addr("0.0.0.0");

	int ret_bind=bind(sockfd,(SA *)&my_addr,sizeof(my_addr));
	if(ret_bind<0)
	{
		perror("fail to bind");
		exit(1);
	
	
	
	}

	char buf[128];

	socklen_t len=sizeof(peer_addr);


	bzero(&peer_addr,sizeof(peer_addr));

	peer_addr.sin_family=AF_INET;
	peer_addr.sin_port=htons(atoi(argv[2]));
	peer_addr.sin_addr.s_addr=inet_addr(argv[1]);


	int ret_connect=connect(sockfd,(SA*)&peer_addr,len);
	if(ret_connect<0)
	{
		perror("fail to connect");
		exit(1);
	
	
	}



	while(1)
	{
		bzero(buf,sizeof(buf));

		fgets(buf,sizeof(buf),stdin);


		send(sockfd,buf,strlen(buf),0);

		bzero(buf,sizeof(buf));

		recv(sockfd,buf,sizeof(buf),0);
	
		printf("%s\n",buf);
	}


	return 0;
}
