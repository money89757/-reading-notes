#include<stdio.h>

int main(int argc, const char *argv[])
{
	printf("hello");
	fflush(stdout);//刷新缓存区
	while(1);
	return 0;
}
