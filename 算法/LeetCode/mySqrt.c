#include <stdio.h>

int mySqrt(int x)
{
	long long i = 0;
	long long j = x/2+1;
	while(i <= j)
	{
		long long mid = (i+j) / 2;
		long long sq = mid * mid;
		if(sq == x)
			return mid;
		else if(sq < x)
			i = mid + i;
		else
			j = mid - 1;
	} 

	return j;
}


int main(int argc, const char *argv[])
{
	int num;
	scanf("%d",&num);
	int number = mySqrt(num);

	printf("mySqrt = %d",number);
	return 0;
}
