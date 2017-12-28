#include<stdio.h>
#include<time.h>

int main(int argc, const char *argv[])
{
	FILE *fp;
	int count = 0;
	if(argc < 2)
	{
		fprintf(stderr,"error\n");
		return -1;
	}

	fp = fopen(argv[1],"w");
	if(fp == NULL)
	{
		perror("fopen");
		return -1;
	}

	time_t t;
	struct tm *ptm;
	while(1)
	{
		count ++;
		t = time(NULL);
		ptm = localtime(&t);

		fprintf(fp,"%d-%d-%d\n",ptm->tm_year + 1900,ptm->tm_mon + 1,ptm->tm_mday);
		fflush(fp);

		sleep(1);

		if(count == 3)
			break;
	}
	fclose(fp);
	return 0;
}
