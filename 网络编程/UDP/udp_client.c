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

#define SA struct sockaddr

int main(int argc, const char *argv[])
{

	//./a.out 172.21.1.250 50000
	if(argc!=3)
	{
		printf("please input ip and port\n");
				return -1;
	
	
	}
	int sockfd=socket(AF_INET,SOCK_DGRAM,0);
	
	if(sockfd<0)
	{
		perror("fail to socket");
		exit(1);
	
	}
	struct sockaddr_in my_addr,peer_addr;
	
	bzero(&my_addr,sizeof(my_addr));

	my_addr.sin_family=AF_INET;
	my_addr.sin_port=htons(60000);
	my_addr.sin_addr.s_addr=inet_addr("0.0.0.0");


	int ret_bind=bind(sockfd,(SA *)&my_addr,sizeof(my_addr));
	if(ret_bind<0)
	{
		perror("fail to bind");
		printf("errno=%d\n",errno);


	}


	char buf[128];
	bzero(&peer_addr,sizeof(peer_addr));
	peer_addr.sin_family=AF_INET;
	peer_addr.sin_port=htons(atoi(argv[2]));

	// argv[2]  -->"50000"
	// atoi   --> ascii to int  " 50000"-->50000  小端
	// htons   little edian  --> big edian 

	peer_addr.sin_addr.s_addr=inet_addr(argv[1]);


	while(1)
	{
		bzero(buf,sizeof(buf));
		fgets(buf,sizeof(buf),stdin);

		//sendto
		sendto(sockfd,buf,strlen(buf),0,(SA *)&peer_addr,sizeof(peer_addr));
		//
		//recvfrom
		bzero(buf,sizeof(buf));
		recvfrom(sockfd,buf,sizeof(buf),0,NULL,NULL);

		printf("%s\n",buf);
	
	
	}



	return 0;
}
