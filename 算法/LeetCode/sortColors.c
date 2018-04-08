#include <stdio.h>

#if 0

void sortColors(int *nums, int numsSize)
{
	int l = 0;
	int r = numsSize - 1;
	int i;

	for(i = 0; i <= r; i++)
	{
		if(nums[i] < 1)
		{
			nums[i] = nums[l];
			nums[l] = 0;
			l++;
		}
		else if(nums > 1)
		{
			nums[i] = nums[r];
			nums[r] = 2;
			r--;
			i--;
		}
	}

}

#endif

void sortColors(int *nums, int numsSize)
{
	int red = -1;
	int white = -1;
	int blue = -1;
	int i;

	for(i = 0; i < numsSize; i++)
	{
		if(nums[i] == 0)
		{
			nums[++blue] = 2;
			nums[++white] = 1;
			nums[++red] = 0;
		}

		else if (nums[i] == 1)
		{
			nums[++blue] = 2;
			nums[++white] = 1;
		}
		else if(nums[i] == 2)
		{
			nums[++blue];
		}
	}
}

int main(int argc, const char *argv[])
{
	int str[20] = {1,2,0,1,1,1,2,2,2,0,0,0,1,1,2,2,0,1,0,2};
	int i;
	for(i = 0; i < 20; i++)
	{
		printf("str[] = %d ",str[i]);
	}
	
	sortColors(str,20);

	for(i = 0; i < 20; i++)
	{
		printf("str[] = %d ",str[i]);
	}

	return 0;
}
