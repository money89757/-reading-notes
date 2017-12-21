#include<stdio.h>

int main(int argc, const char *argv[])
{
	FILE *fp;

//	fp = fopen("1.txt","a");//如果文件不存在创建文件，文件存在从尾部添加内容。
	fp = fopen("1.txt","a+");//如果文件不存在创建文件，文件存在从尾部添加内容。
	if(fp == NULL)
	{
		perror("fopen");
		return -1;
	}

	
#if 0
	//如果先读取数据，再写数据，都一定会从文件尾部追加内容
	char ch;
	fscanf(fp,"%c",&ch);

	printf("%c\n",ch);
	
	fprintf(fp,"hello");
#endif
	//先写再读，从文件末尾读取数据
	fprintf(fp,"17022");

	char ch;
	fscanf(fp,"%c",&ch);

	printf("%c\n",ch);

	return 0;
}
