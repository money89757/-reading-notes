#include "db.h"


int opendb(const char * dbname , sqlite3 * db)
{

	int	nret = sqlite3_open(dbname, &db);

	if (nret)
	{
		fprintf(stderr,  "%s\n" , sqlite3_errmsg(db));
		sqlite3_close(db);
		return -1;
	}

	return 0;
}

int closedb(sqlite3 * db)
{
	sqlite3_close(db);
	return 0;
}

static  int  callback(void *NotUsed,int argc, char **argv, char ** azColName)
{
	printf("callback");
	return 0;

}
int  insertsql(sqlite3 *db, const char * sql )
{
 	int  nret;
	char * ErrMsg= NULL;

	nret  =  sqlite3_exec(db,sql,callback,0,&ErrMsg);

	if (nret !=SQLITE_OK)
	{
		fprintf(stderr, "SQL error:%d, %s\n",nret,ErrMsg);
		sqlite3_free(ErrMsg);
		
		return  -1;
	}
	
	return 0;

}

