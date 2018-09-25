#include <stdio.h>

int removeDuplicates(int *nums, int numsSize)
{
	if(nums == NULL || numsSize == 0)
		return 0;

	int right = 0,i;
	for(i = 0; i < numsSize; i++)
	{
		if(right < 2 || nums[right - 2] != nums[i])
			nums[right++] = nums[i];
	}
	return right;
}

int main(int argc, const char *argv[])
{
	int nums[] = {0,0,1,1,1,1,2,3,3};
	int a,i;
	a = removeDuplicates(nums,9);
	printf("a = %d\n",a);

	for(i = 0; i < a ; i++)
	{
		printf("nums[%d] = %d\n",i,nums[i]);
	}
	return 0;
}
