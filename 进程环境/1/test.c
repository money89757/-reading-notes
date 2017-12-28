#include<stdio.h>
#include<stdlib.h>
int fun()
{
	printf("start\n");
//	exit(0); //结束一个进程的
	return 0;//结束一个函数
}

int main(int argc, const char *argv[])
{
	fun();
	printf("end\n");
	return 0;
}
