#include <stdio.h>

int removeDuplicates(int *nums, int numsSize)
{
	if(nums == NULL || numsSize == 0)
		return 0;

	int i = 0;
	int j = 0;

	for(; i < numsSize; i++)
	{
		if(nums[j] != nums[i])
			nums[++j] = nums[i];
	}

	return j+1;
}

int main(int argc, const char *argv[])
{
	int str1[4] = {1,1,2,2};
	int str2[4] = {0,0,0,0};
	int str3[3] = {1,1,1};
	int str4[1] = {0};
	int str5[5] = {1,2,3,4,5};

	int i,j,k,a,b;

	i = removeDuplicates(str1,4);
	j = removeDuplicates(str2,4);
	k = removeDuplicates(str3,3);
	a = removeDuplicates(str4,1);
	b =removeDuplicates(str5,5);

	printf("%d %d %d %d %d\n",i,j,k,a,b);
	return 0;
}
