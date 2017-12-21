#include<stdio.h>

int main(int argc, const char *argv[])
{
	FILE *fp;

	fp = fopen("1.txt","w");

	fprintf(fp,"hello");
	
	fclose(fp);//表面是关闭了一个文件指针，深层的含义是刷新缓存区
	while(1);
	return 0;
}
