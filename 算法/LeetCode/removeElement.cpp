#include <stdio.h>

int removeElement(int *nums, int numsSize,int val)
{
	int res = 0;
	int i;
	for(i = 0; i < numsSize; i++)
	
	if(nums[i] != val)
		nums[res++] = nums[i];
		
	return res;
}

int main()
{
	int str[4] = {3,2,2,3};
	int num = removeElement(str,4,2);
	
	printf("num = %d\n",num);
}
