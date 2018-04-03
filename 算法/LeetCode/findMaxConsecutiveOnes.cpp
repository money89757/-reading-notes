#include <stdio.h>

int findMaxConsecutiveOnes(int* nums, int numsSize) 
{
    int max = 0;
	int one = 0;
	for(int i = 0; i < numsSize; i++)
	{
		if(nums[i] == 1)
		{
			one++;
			if(one > max)
			max = one;
		}		
		else if(nums[i] == 0)
		{
			if(one>max)
			max = one;
			one = 0;
		}
	}    
	
	return max;
	
}

int main(void)
{
	int a[10] = {0,1,1,1,1,1,0,0,1,1};
	int ret = findMaxConsecutiveOnes(a,10);
	
	printf("%d ",ret);
}
