#include <stdio.h>

int climbStairs(int n)
{
	if(n == 0)
		return 0;
	if(n == 1)
		return 1;
	if(n == 2)
		return 2;
	
	int a = 1;
	int b = 2;
	int c = a + b;

	while(n > 3)
	{
		a = b;
		b = c;
		c = a + b;
		n--;
	}
	return c;
}

int main(int argc, const char *argv[])
{
	int c,d;
	c = climbStairs(10);
	d = climbStairs(0);
	printf("%d %d ",c,d);
	return 0;
}
