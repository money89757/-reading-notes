#include<stdio.h>

int main(int argc, const char *argv[])
{
	FILE *fp;

//	fp = fopen("2.txt","w");//以只写的方式打开文件，如果文件不存在创建，文件存在则清空文件内容
	fp = fopen("1.txt","w+");//以读写的方式打开文件，如果文件不存在创建，文件存在则清空文件内容
	if(fp == NULL)
	{
		perror("fopen");
		return -1;
	}

	fprintf(fp,"17022");
	fprintf(fp,"17022");
	
	rewind(fp);
	char buf[128];
	fscanf(fp,"%s",buf);
	printf("%s\n",buf);
	return 0;
}
