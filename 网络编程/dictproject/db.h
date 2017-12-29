#ifndef _DB_H_
#define _DB_H_

#include<sqlite3.h>
#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>

typedef struct
{
	char wordname[200];  //word 
	char worddesc[300];  //description
}  word_t;


//static int  readline(int fd, char * buf,int * nsize);

//static void  wordcpy(char * desc  , const  char* src);

int  insertsql(sqlite3 *db, const char * sql );

//int loaddb(const char * dbname, const char *dictname);


#endif
