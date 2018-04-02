#include <stdio.h>

int singleNumber(int *nums, int numsSize)
{
	int i;
	int ret = 0;

	for(i = 0; i < numsSize; i++)
	{
		ret = nums[i] ^ ret;
	}

	return ret;
}

int main(int argc, const char *argv[])
{
	int a[5] = {0,0,2,1,1};
	int ret = singleNumber(a,5);
	printf("ret = %d",ret);
	
	return 0;
}
