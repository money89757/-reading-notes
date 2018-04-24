#include <stdio.h>

#define bool int

#define false 0
#define true 1

int isPowerOfThree(int n)
{
	if(n == 1)
		return true;
	else if(n == 0)
		return false;
	else if(n % 3 == 0)
		return isPowerOfThree(n / 3);
	else
		return false;
}

int main(int argc,const int **argv[])
{
	int nums;
	scanf("%d",&nums);

	printf("\n %d \n",isPowerOfThree(nums));
}

