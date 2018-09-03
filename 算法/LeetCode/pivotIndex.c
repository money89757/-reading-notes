#include <stdio.h>

int pivotIndex(int *nums, int numsSize)
{
#if 0
	if( nums == NULL || numsSize == 0)
		return 0;
	 
	int leftSum = 0;
	int rightSum = 0;
	int sum = 0;
	int i,n;
	for(n = 1; n < numsSize; n++)
	{
		sum += n;
	}

	printf("sum = %d\n",sum);
	for(i = 0; i < numsSize; i++)
	{
		if(i == 0){
			leftSum = 0;
		}else{
			leftSum += leftSum + nums[i - 1];
		}
		rightSum = sum - leftSum - nums[i];
		if(leftSum == rightSum){
			return i;
		}
		printf("i = %d\n",i);

	}

	return -1;
#endif 
	if(nums == NULL || numsSize == 0)
		return -1;
	int leftSum,rightSum;
	int i;
	leftSum = 0;
	rightSum = 0;

	for(i = 1; i < numsSize; i++)
		rightSum += nums[i];

	for(i = 0; i < numsSize; i++)
	{
		if(leftSum == rightSum)
			return i;

		leftSum += nums[i];
		rightSum -= nums[i + 1];
	}

	return -1;
}

int main(int argc, const char *argv[])
{
	int a[6] = {1,7,3,6,5,6};
	int num;
	
	num = pivotIndex(a,6);

	printf("num = %d\n",num);

	return 0;
}
