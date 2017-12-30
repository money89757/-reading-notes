#include <stdio.h>
#include <sys/types.h>	       /* See NOTES */
#include <sys/socket.h>
#include <strings.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>



int main(int argc, const char *argv[])
{
	int sockfd=socket(AF_INET,SOCK_DGRAM,0);
	
	if(sockfd<0)
	{
		perror("fail to socket");
		exit(1);
	
	}
	struct sockaddr_in my_addr,peer_addr;
	socklen_t len=sizeof(peer_addr);
	//memset   void * , 0, sizeof();
	bzero(&my_addr,sizeof(my_addr));
	
	my_addr.sin_family=AF_INET;
	my_addr.sin_port=htons(50000);
	my_addr.sin_addr.s_addr=inet_addr("0.0.0.0");
	//
	int ret_bind=bind(sockfd,(struct sockaddr*)&my_addr,sizeof(my_addr));
	if(ret_bind<0)
	{
		
		perror("fail to bind");
		exit(1);
	
	
	}

	char buf[128];
	char sendbuf[128];
	while(1)
	{
		bzero(buf,sizeof(buf));
		bzero(&peer_addr,len);

		recvfrom(sockfd,buf,sizeof(buf),0,(struct sockaddr*)(&peer_addr),&len);


		bzero(sendbuf,sizeof(sendbuf));

		sprintf(sendbuf,"ip:%sport:%d buf:%s",inet_ntoa(peer_addr.sin_addr),ntohs(peer_addr.sin_port),buf);

		printf("%s\n",sendbuf);
		//  ntohs  network to host short
		//
		//  inet_ntoa   network to ascii
		//
		//

		peer_addr.sin_port=htons(60000);

		int i;
		for(i=2;i<=254;i++)
		{
		//	"172.22.1.x"   --> 172* 2^24+ 22* 2^ 16+1*2^8+ x  --> bigedian 
		//
			peer_addr.sin_addr.s_addr=htonl((172<<24)+(22<<16)+(1<<8)+i);
			//htonl   host to network long
	
			sendto(sockfd,sendbuf,strlen(sendbuf),0,(struct sockaddr*)&peer_addr,len);
	
		}
	
	}



	return 0;
}
