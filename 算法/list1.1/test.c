#include <stdlib.h>
#include <stdio.h>

int main(int argc, const char *argv[])
{
	
	struct s{
		int a;
		int b;
		int data[0];
	}x;

	struct d{
		int size;
		int data[0];
	}*dp;

	dp = malloc(sizeof(struct d) + sizeof(int ) * 100);

	struct s *p;
	p = &x;

	struct ssss *l;
	//struct ssss a;

	l = malloc(100);

	//l->a = 3;

	int data[0];

	printf("sizeof(data) = %ld\n",sizeof(data));
	printf("sizeof(x) = %ld\n",sizeof(x));

	data[12] = 30;

	return 0;
}
