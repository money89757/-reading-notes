#include<stdio.h>

int main(int argc, const char *argv[])
{
	FILE *fp;

	fp = fopen("1.txt","r");
	if(fp == NULL)
	{

	}
	
	int ch;

	while(1)
	{
		ch = fgetc(fp);
		if(ch == EOF && feof(fp) == 0)
		{
			fprintf(stderr,"error\n");
			fclose(fp);
			return -1;
		}
		if(ch == EOF && feof(fp) != 0)
		{
			printf("文件到达尾部\n");
			break;
		}
		else 
			printf("%d\t",ch);
	}
	fclose(fp);
	return 0;
}
