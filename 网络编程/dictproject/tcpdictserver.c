#include<stdio.h>
#include<sys/types.h>	      
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/select.h>
#include<strings.h>
#include"tcpmsg.h"
#include<time.h>
#include<sqlite3.h>
#include <string.h>

int  sockfd;
char strsql[1024];
const char * dbname;
int do_register(int fd, MSG * pmsg);
int do_login(int fd, MSG * pmsg);
int do_query(int fd, MSG * pmsg);
int do_history(int fd, MSG * pmsg);

int do_client(int , MSG *);

int main(int argc, const char *argv[])
{
	int socknewfd,nlen,nRet;
	int optVal = 1;
	MSG  msg;

	struct  sockaddr_in  addrSvr , addrClient;

	if( argc < 2)
	{
		printf("Useage: tcpdictserver  <dbname>\n");
		return -1;
	}

	dbname  = argv[1];

	addrSvr.sin_family = AF_INET;
	addrSvr.sin_port  =  htons(50000);
	addrSvr.sin_addr.s_addr  = inet_addr("0.0.0.0");

	sockfd = socket(AF_INET,SOCK_STREAM,0);

	if(sockfd <0)
	{
		perror("fail to  sockect");
		return  -1;
	}

	setsockopt (sockfd , SOL_SOCKET, SO_REUSEADDR, &optVal , sizeof(optVal));

	 nRet  =  bind(  sockfd, (struct sockaddr *)  & addrSvr, sizeof(addrSvr));


	if (nRet<0)
	{
		perror("fail to bind");

		close(sockfd);
		return  -1;
	
	}

	nRet = listen(sockfd, 10);

	if(nRet < 0)
	{
		perror("fail to listen");
		close(sockfd);
		return -1;
	
	}



	nlen =  sizeof(addrClient);

	bzero(&addrClient,sizeof(addrClient));


	fd_set  sockfds;
	fd_set  clientfds;

	int nMax  =  sockfd;
	int index =0;

	FD_ZERO(&sockfds);
	FD_ZERO(&clientfds);
	
	FD_SET(sockfd, &clientfds);

	

	while(1)
	{
	
		sockfds = clientfds;
		
		select(nMax + 1 ,  &sockfds ,NULL,NULL,NULL);

		
		for(index =3 ; index <  nMax +1 ;  index ++)
		{
		
			if(FD_ISSET(sockfd, &sockfds) &&  index == sockfd)
			{
				socknewfd  = accept(sockfd,(struct sockaddr *)  &addrClient , &nlen);

				FD_SET(socknewfd, &clientfds);

				if (socknewfd  >  nMax)  nMax  = socknewfd;

			}
			
			else if (FD_ISSET(index, &sockfds))
			{
				bzero(&msg,sizeof(msg));

			  nRet  = recv(index, &msg, sizeof(msg),0);

			    if( nRet  < 0)
				{
					perror("fail to recv");
					
					close(index);
						
					nMax =  getMax(&clientfds , index , nMax);

					FD_CLR(index , & clientfds);
					
					continue;
				}	
				
				if (nRet == 0)
				{
					printf("client  %d  is close!\n" , index);
					
					close(index);				
					
					nMax =  getMax(&clientfds , index , nMax);

					FD_CLR(index , & clientfds);

					continue;
				}	
				
										
				do_client(index,&msg);
			
			}

		}

	}

	close(sockfd);
	return 0;
}


int  getMax(fd_set *  fdset, int index , int nMax)
{
	int i  =0;

	if (index <  nMax) return  nMax;

	for (i =nMax; i>3; i--)
	{

		if (i <  index &&  FD_ISSET(i ,fdset))  
		{
			return  nMax;
		}
		
	
	}
	
}

int do_client(int fd, MSG *  pmsg)
{
	
	switch( pmsg->type)
	{
		case R:  //register
			do_register(fd, pmsg);
			break;
		case L:  // login
			do_login(fd, pmsg);
			break;
		case Q: //query
			do_query(fd,pmsg);
			break;	
		case H:
			do_history(fd,pmsg);
			break;
	}

	return 1;
}

int do_register(int fd, MSG * pmsg)
{
	bzero(strsql,1024);

	sqlite3 * db=NULL;
	
	char * ErrMsg;


	int nret = sqlite3_open(dbname , &db);

	if(nret)
	{
		fprintf(stderr,"%s\n", sqlite3_errmsg(db));
		return -1;
	}


	printf("New  User Info <REGISTER> \n");
	printf("name=%s\n",pmsg->name);
	printf("password=%s\n",pmsg->data);

	sprintf(strsql, "insert into userinfo values(null,'%s','%s')",pmsg->name, pmsg->data);
	
	bzero(pmsg, sizeof(MSG));
	pmsg->type = R;
	
	nret  = sqlite3_exec(db, strsql, NULL,0, &ErrMsg);

	if (nret!=SQLITE_OK)
	{
		if(nret == 19)
			strcpy(pmsg->data, "already exist");
		else
			sprintf(pmsg->data,"SQL ERROR:%d, %s\n",nret,ErrMsg);
		
		sqlite3_free(ErrMsg);

		pmsg->name[0] ='0';
	} 
	else
	{
 		strcpy(pmsg->data, "OK!");
		pmsg->name[0]='1';
	}

	send(fd, pmsg, sizeof(MSG),0);

	sqlite3_close(db);

}

int do_login(int fd, MSG * pmsg)
{
	bzero(strsql,1024);

	sqlite3 * db=NULL;
	sqlite3_stmt *statement;

	char * ErrMsg;
	
	

	int nret = sqlite3_open(dbname , &db);

	if(nret)
	{
		fprintf(stderr,"%s\n", sqlite3_errmsg(db));
		return -1;
	}


	printf("User Info <LOGIN> \n");
	printf("name=%s\n",pmsg->name);
	printf("password=%s\n",pmsg->data);

	sprintf(strsql, "select id  from  userinfo where loginName='%s' and  password='%s'",pmsg->name, pmsg->data);
	
	bzero(pmsg, sizeof(MSG));

	pmsg->type =L;
	
	nret  = sqlite3_prepare_v2(db, strsql, 1024,&statement, NULL);

	if (nret!=SQLITE_OK)
	{
		strcpy(pmsg->data, " prepare_v3 Error");

		pmsg->name[0] ='0';
	} 
	else
	{
		if (sqlite3_step(statement)!=SQLITE_ROW)
		{
			
			strcpy(pmsg->data, "User/password Error!");

			pmsg->name[0] ='0';
		} 
		else
		{
			strcpy(pmsg->data, "Login OK!");
			pmsg->name[0]='1';
		}
	}

	send(fd, pmsg, sizeof(MSG),0);
 	sqlite3_finalize(statement);
	sqlite3_close(db);

}


int do_query(int fd, MSG * pmsg)
{
	bzero(strsql,1024);

	sqlite3 * db=NULL;
	sqlite3_stmt *statement;

	char * ErrMsg;
	
	time_t now;

	int nret = sqlite3_open(dbname , &db);

	if(nret)
	{
		fprintf(stderr,"%s\n", sqlite3_errmsg(db));
		return -1;
	}


	printf("query word  <QUERY> \n");
	printf("word=%s\n",pmsg->data);

	sprintf(strsql, "select desc from  dictinfo where word='%s'", pmsg->data);
	
	
	nret  = sqlite3_prepare_v2(db, strsql, 1024,&statement, NULL);

	bzero(strsql,sizeof(strsql));
	
	time(&now);

	sprintf(strsql,"insert into record values(null,'%s','%s','%s')",pmsg->name, pmsg->data, asctime(localtime(&now)));
	
	//printf("%s\n",strsql);	
	bzero(pmsg, sizeof(MSG));

	pmsg->type =Q;

	if (nret!=SQLITE_OK)
	{
		strcpy(pmsg->data, " prepare_v3 Error");

		pmsg->name[0] ='0';
	} 
	else
	{
		if (sqlite3_step(statement)!=SQLITE_ROW)
		{
			
			strcpy(pmsg->data, "Not Found!");

			pmsg->name[0] ='0';
		} 
		else
		{

			strcpy(pmsg->data, (char *) sqlite3_column_text(statement,0));

			//insert  history 

			sqlite3_exec(db,strsql,NULL,0,NULL);

			pmsg->name[0]='1';
		}
	}

	send(fd, pmsg, sizeof(MSG),0);
 	sqlite3_finalize(statement);
	sqlite3_close(db);

}

int do_history(int fd, MSG * pmsg)
{

	sqlite3 * db=NULL;
	sqlite3_stmt *statement;

	char * ErrMsg;

	int nret = sqlite3_open(dbname , &db);

	if(nret)
	{
		fprintf(stderr,"%s\n", sqlite3_errmsg(db));
		return -1;
	}
	

	nret  = sqlite3_prepare_v2(db, "select * from record", 100,&statement, NULL);

	bzero(pmsg, sizeof(MSG));

	pmsg->type =H;

	if (nret!=SQLITE_OK)
	{
		strcpy(pmsg->data, " prepare_v3 Error");
		pmsg->name[0] ='0';
		send(fd, pmsg, sizeof(MSG),0);
	} 
	else
	{
		while(sqlite3_step(statement)==SQLITE_ROW)
		{
			bzero(pmsg, sizeof(MSG));
			
			sprintf(pmsg->data ,"%d\t%s\t%s\t%s\n ",sqlite3_column_int(statement,0)
												   ,sqlite3_column_text(statement,1)
												   ,sqlite3_column_text(statement,2)
												   ,sqlite3_column_text(statement,3));
			

			pmsg->type =H;
			pmsg->name[0]='1';
			send(fd, pmsg, sizeof(MSG),0);
		}
	}
	
	bzero(pmsg, sizeof(MSG));
	pmsg->type =H;
	send(fd, pmsg, sizeof(MSG),0);
	
 	sqlite3_finalize(statement);
	sqlite3_close(db);

}
