#include <stdio.h>
#include <string.h>


int dominantIndex(int *nums, int numsSize)
{
//	if(nums == NULL || numsSize == 0)
//		return -1;
#if 0
	int i,j,temp;

	for(i = 0; i < numsSize - 1; i++)
	{
		for(j = 0; j < numsSize - i - 1; j++)
		{
			if(nums[j] > nums[j + 1])
			{
				temp = nums[j + 1];
				nums[j + 1] = nums[j];
				nums[j] = temp;
			}
		}
	}

	for(i = 0; i < numsSize; i++)
	{
		printf("%d\n",nums[i]);
	}
#endif

#if 0
	int max1 = nums[numsSize - 1];
	int max2 = nums[numsSize - 2];
	printf("max1 = %d\n max2 = %d\n",max1,max2);


	if(max2 != 0 && max1/max2 < 2)
		return -1;

	i = 0;
	for(j = 0; j < numsSize; j++)
	{
		if(nums[j] == max1)
			i = j;
		return i;
	}
	return i;
#endif 

#if 0
	int max = nums[numsSize - 1];
	i = 0;
	for(j = 0; j < numsSize;j++)
	{
		if(nums[j]!=max && nums[j] != 0 && max/nums[j] < 2)
			return -1;
		else if(nums[j] == max) 
			i = j;
	}
	return i;
#endif
	
	if(nums == NULL || numsSize == 0)
		return -1;
	if(numsSize == 1)
		return 0;

	int max1=0;
	int max2 = 0;
	int i,k = 0;
	for(i = 0; i < numsSize; i++)
	{
		if(nums[i] > max1)
		{
			max2 = max1;
			max1 = nums[i];
			k = i;
		}
		else if(nums[i] > max2)
		{
			max2 = nums[i];
		}
	}

	if(max2 == 0 || max1 >= 2*max2)
		return k;
	
	else
		return -1;

}


int main(int argc, const char *argv[])
{
	int str[] = {3,6,0,1};

	int num;

	num = dominantIndex(str,4);

	printf("num = %d\n",num);
	return 0;
}
