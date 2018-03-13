#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main(int argc, const char *argv[])
{
	time_t ts;
	unsigned int data = time(&ts);
	srand(data);
	
	int a[10];

	int max = a[0];

	for(int i = 0; i < 10; i++)
	{
		a[i] = rand() % 100;
		printf("\n%d",a[i]);
	}

	for(int i = 0; i < 10; i++)
	{
		if(a[i] > max)
		{
			max = a[i];
		}
	}

	printf("big = %d\n",max);

	
	int min = a[0];
	for(int i = 0; i < 10; i++)
	{
		if(a[i] < min)
		{
			min = a[i];
		}
	}
	printf("small = %d\n",min);
}



#if 0
int main(int argc, const char *argv[])
{
	time_t ts;
	unsigned int data = time(&ts);
	srand(data);

	int a[10];

	for(int i = 0; i < 10; i++)
	{
		a[i] = rand() % 300;
		printf("\n%d\n",a[i]);
	}

	int num;
	scanf("%d",&num);
	int flag = 0;

	for(int i = 0; i < 10; i++)
	{
		if(num == a[i])
		{
			flag = 1;
			break;
		}
	}
		if(flag)
			printf("right\n");
		else
			printf("error");
	return 0;
}
#endif


