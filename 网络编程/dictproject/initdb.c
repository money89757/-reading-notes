#if 0
#include<sqlite3.h>
#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#endif

#include "db.h"

static int loaddb(const char * dbname, const char *dictname);

static int  readline(int fd, char * buf,int * nsize);

static void  wordcpy(char * desc  , const  char* src);

int main(int argc, const char *argv[])
{


	if( argc < 3)
	{
		printf("usage: initdb <dbname> <dictname>\n");
		return -1;
	}


	loaddb(argv[1],argv[2]);
	return 0;
}


static int loaddb(const char * dbname, const char *dictname)
{

	char  buf[1024], strsql[1024];
	char *p = NULL;

	int dicfd,nret,nlen=0,nword=0;
	
	word_t wordline;
		
	sqlite3 * db;


	nret = sqlite3_open(dbname, &db);

	if (nret)
	{
		fprintf(stderr,  "%s\n" , sqlite3_errmsg(db));
		sqlite3_close(db);
		return -1;
	}


	dicfd=open(dictname , O_RDONLY); //dict file

	if( dicfd <0)
	{
		perror("open dict");
		return -1;
	}

	insertsql(db, "delete from  dictInfo");

	nret  = 1;
	while(nret > 0)
	{
		nlen = sizeof(buf);

		bzero(buf , nlen);

		nret = readline(dicfd , buf, &nlen);

		if( nlen > 0)
		{
			// get  word 
			nword ++;
			p  =  strtok(buf,  " ");		
			bzero( &wordline, sizeof(wordline));
			if(p==NULL) break;
			wordcpy(wordline.wordname, p);
			if(strlen(wordline.wordname) < 1) break;
			//printf("%d,NAME=%s,%d\n", nword,wordline.wordname,strlen(wordline.wordname));


			// get word's  description	
			p =  strtok(NULL, "\n");
			if(p==NULL) break;
			wordcpy(wordline.worddesc, p);
			//printf("DESC=%s\n", wordline.worddesc);
			
			bzero(strsql, sizeof(strsql));

			sprintf(strsql,"insert into dictInfo values(%d,'%s','%s')",nword,wordline.wordname,wordline.worddesc);
			insertsql(db,strsql);
		
		}		

	}

	close(dicfd);
	sqlite3_close(db);

	return 0;
}

//get a line
static int  readline(int fd, char * buf,int * nsize)
{	
	char  c;
	int nret,index=0;

	bzero(buf, *nsize);

	while(1)
	{
		nret  = read(fd,  &c,  1);

		if (nret < 1)
		{ 
			*nsize = index;

			break;
		}

		buf[index] = c;
		
		if(c=='\r') 
		{
			*nsize = index+1;
			break;	
		}


		index ++;
	}

	return nret;
}


static void  wordcpy(char * desc  , const  char* src)
{
	int i=0, index=0;
	char c= ' ';

	while( src[i]!='\0')
	{
		if( c==' ') 
		{
			if (src[i] ==' ')
			{			
				i++;
				continue;
			}
			else
				c = src[i];	
		}

		switch(src[i])
		{
		case '\n':
			i++;
			continue;
		case ',':
			desc[index] ='#';
			break;

		case '\'':
			desc[index] ='$';
			break;

		case '(':
			desc[index] ='@';
			break;

		case ')':
			desc[index] ='_';
			break;
			
		default:
			desc[index] = src[i];
			break;

		}

		index++;
		i++;
	}

}


