#include <stdio.h>
#include <stdlib.h>

int *twoSum(int *nums, int numsSize, int target)
{
	int *twosum = (int *)malloc(sizeof(int));
	int i;
	int j;
	
	for(i = 0; i < numsSize; i++)
	{
		for(j = i+1; (j < numsSize && j != i); j++)
		{
			if(nums[i] + nums[j] == target)
			{
				twosum[0] = i;
				twosum[1] = j;
			}
		}
	}
	
	return twosum;
}

int main()
{
#if 0
    int nums[4] = {2,7,11,15};
    int target = 26;
    int count = sizeof(nums)/sizeof(*nums);
#endif
    
#if 0
    int nums[4] = {0,7,11,0};
    int target = 0;
    int count = sizeof(nums)/sizeof(*nums);
#endif
    
	int nums[4] = {-1,-7,11,0};
    int target = -8;
    int count = sizeof(nums)/sizeof(*nums);
	
    int *index = twoSum(nums, count, target);
    if(index != NULL)
    	printf("%d %d\n",index[0], index[1]);
    
    return 0;
    
}
