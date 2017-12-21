#include<stdio.h>

int main(int argc, const char *argv[])
{
//	printf("hello\n");
//	while(1);
	int a = 1;	
	while(1)
	{
//		printf("hello");
		printf("%0256d",a);
	//	usleep(100000);
		sleep(1);
	}
	return 0;
}
