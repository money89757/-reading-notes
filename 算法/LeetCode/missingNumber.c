#include <stdio.h>

int missingNumber(int *nums, int numsSize)
{
#if 0
	if(nums == NULL || numsSize == 0)
		return 0;

	int total = 0;
	int incomplete = 0;
	int i;

	for(i = 0; i < numsSize; i++)
	{
		total = total ^ i;
	}

	for(i = 0; i < numsSize; i++)
	{
		incomplete = incomplete ^ nums[i];
	}
	printf("\n %d ", incomplete^total);
	return incomplete ^ total;
#endif
	if(nums == NULL || numsSize == 0)
		return 0;

	int i = 0;

	int sum = numsSize * ( numsSize + 1 )/2;
	for(i = 0; i < numsSize; i++)
	{
		sum -= nums[i];
	}
	return sum;
}

int main(int argc, const char *argv[])
{
	int a[6] = {3,0,1,2,5,3};
	int i;
	int ret;

	for(i = 0; i < 6; i++)
	{
		printf("%d ",a[i]);
	}

	ret = missingNumber(a, 6);
	
	printf("\nret = %d\n",ret);

	return 0;
}
