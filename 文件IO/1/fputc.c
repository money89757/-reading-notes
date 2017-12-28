#include<stdio.h>

int main(int argc, const char *argv[])
{
	FILE *fp;

	fp = fopen("1.txt","w");

	fputc('A',fp);
	fputc(65,fp);
//	fputc(650,fp); 第一个参数在使用时不要超过ascii的值域

	fclose(fp);
	return 0;
}
