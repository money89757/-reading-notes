#include<stdio.h>

int main(int argc, const char *argv[])
{
	FILE *fp;
	char ch;
#if 0
	if(argc < 2)
	{
		fprintf(stderr,"argv input error\n");
		return -1;
	}

//	fp = fopen("2.txt","r"); //只读的方式打开文件，并且文件必须事先存在
	fp = fopen(argv[1],"r");
	if(fp == NULL)
	{
//		printf("error\n");
//		fprintf(stdout,"error\n");	
//		fprintf(stderr,"error\n");//报错的第一种情况,比较通用
		perror("fopen");//这种报错方式只能用于函数返回值出错的情况
		return -1;
	}

	fscanf(fp,"%c",&ch);
	printf("%c\n",ch);


	fscanf(fp,"%c",&ch);
	printf("%c\n",ch);
//	fp ++;//文件指针不能自增和自减
#endif

	fp = fopen("1.txt","r+");
	if(fp == NULL)
	{
		perror("fopen");
		return -1;
	}

	char buf[128];
	fprintf(fp,"12345");

	rewind(fp);//将文件指针重新定位到文件开头

	fscanf(fp,"%s",buf);
	printf("%s\n",buf);
	return 0;
}

