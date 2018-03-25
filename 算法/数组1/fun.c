#include <stdio.h>
#include <stdlib.h>

#if 0
int main(int argc, const char *argv[])
{
	
	char a[5] = {1,2,3,4,5};
	int i = 0;

	for(i = 0; i < 5; i++)
	{
		printf("\n%d,%p",a[i],&a[i]);
	}

	return 0;
}



int main(int argc, const char *argv[])
{
	double a[5] = {4.3,5.4,5.7,5.7,3.2};
	int i = 0;
	for(i = 0;i < 5; i++)
	{
		printf("\n%f,%f,%f,%p,%p",a[i],*(&a[i]),*(a+i),&a[i],a+i);
	}
	return 0;
}

#endif

int main()
{
	char a[10] = {1,2,3,4,5};

	int i = 0;
	for(i = 0;i < 10; i++)
	{
		printf("\n%p",&a[i]);
	}
}
