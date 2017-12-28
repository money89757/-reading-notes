#include<stdio.h>
#include<unistd.h>

int main(int argc, const char *argv[])
{
	fork();
	fork();
	printf("hello\n");
	while(1);
	return 0;
}
