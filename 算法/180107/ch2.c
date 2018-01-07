#include <stdio.h>
#include <stdlib.h>


int main(int argc, const char *argv[])
{
	
	
	int *p;
	p = (int *)malloc(sizeof(int));
	*p = 10;

	printf("*p = %d\n",*p);

	getchar();
	getchar();
	return 0;
}
