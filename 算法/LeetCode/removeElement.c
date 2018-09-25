#include <stdio.h>

int removeElement(int *nums, int numsSize, int val)
{
	if(nums == NULL || numsSize == 0)
		return 0;

	int res,i;
	for(i = 0; i < numsSize; i++)
	{
		if(nums[i] != val)
			nums[res++] = nums[i];
	}

	return res;
}
