#include <stdio.h>
#include <limits.h>

int maxSubArray(int *nums, int numsSize)
{
	if(numsSize == 0 || nums == NULL)
		return 0;

	int sum = 0;
	int maxsum = INT_MIN;
	int i;

	for(i = 0; i < numsSize; i++)
	{
		sum += nums[i];
		if(sum > maxsum)
			maxsum = sum;
		if(sum < 0)
			sum = 0;
	}

	return maxsum;
}

int main(int argc, const char *argv[])
{
	int str[9] = {-2,1,-3,4,-1,2,1,-5,4};
	int n;
	n = maxSubArray(str,9);
	printf("maxSubArray = %d\n",n);
	return 0;
}
