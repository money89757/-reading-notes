#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(int argc, const char *argv[])
{
	printf("hello");
//	exit(EXIT_SUCCESS);//只需要关心成功传递EXIT_SUCCESS 失败传递EXIT_FAILURE
						//退出进程时会刷新流缓存区
	
	_exit(EXIT_SUCCESS);//这个函数了解就可以
	//总结:exit函数和_exit函数都可以退出进程，_exit退出时不刷新缓存区
	while(1);
}
