#include <stdio.h>

int main(int argc, const char *argv[])
{
	int a = 10;
	int *p;
	p=&a;
	printf("a place = %d\n",*p);

	
	printf("a place = %d\n",&a);

	return 0;
}
