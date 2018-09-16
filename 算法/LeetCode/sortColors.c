#include <stdio.h>
#if 0
void swap(int *nums, int i, int j)
{
	int tmp = nums[i];
	nums[i] = nums[j];
	nums[j] = tmp;
}

void quicksort(int *nums, int left, int right)
{
	int partition = 1;
	int l = left;
	int r = right;
	int i = left;
	while(i <= r)
	{
		if(nums[i] < partition)
		{
			swap(nums, i, l++);
			i++;
		}
		else if(nums[i] > partition)
		{
			swap(nums, i, r--);
		}
		else 
		{
			i++;
		}
	}
}

void sortColors(int *nums, int numsSize)
{
	quicksort(nums,0,numsSize - 1);
}
#endif

void sortColors(int *nums, int numsSize)
{
	int i,j,temp;

	for(i = 0; i < numsSize; ++i)
	{
		for(j = i + 1; j <numsSize; ++j)
		{
			if(nums[i] > nums[j])
			{
				temp = nums[i];
				nums[i] = nums[j];
				nums[j] = temp;
			}
		}
	}
}

int main(int argc, const char *argv[])
{
	int i;
	int nums[] = {2,0,2,1,1,0};
	sortColors(nums,6);
	for(i = 0; i < 6; i++)
	{
		printf("%d ",nums[i]);
	}
	printf("\n");

	return 0;
}
