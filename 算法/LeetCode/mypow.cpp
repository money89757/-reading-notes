#include <stdio.h>
#include <stdlib.h>

static double fast_pow(double x, int n)
{
	if(n == 0) return 1.0;
	if(n == 1) return x;
	
	double t = fast_pow(x, n/2);
	
	return n & 1 ? t * t * x : t * t;
}

static double mypow(double x, int n)
{
	return n < 0 ? 1/fast_pow(x,-n) : fast_pow(x,n);
}

int main(void)
{
	double x = 2.00000;
	int n = -2;
	
	double mm = mypow(x,n);
	
	printf("%lf\n",mm);
	
}
