#include <stdio.h>
#define max(a,b) (((a) > (b)) ? (a) : (b))

int findMaxConsecuitveOnes(int *nums, int numsSize)
{

	if(nums == NULL || numsSize == 0)
		return 0;

	int max = 0,one = 0;
	int i;
	for(i = 0; i < numsSize; i++)
	{
		if(nums[i] == 1)	
		{
			one ++;
			max = max(max,one);
		}
		else
			one = 0;
	}

	return max;

#if 0
	int *p = nums;
	int *q = nums;
	int count = 0;
	int max = 0;
	
	while(*p != numsSize - 1)
	{
		if(*p = *q)
		{
			count ++;
			p ++;
			if(max <= count)
				max = count;
		}
		else
		{
			count = 0;
			q = p;
		}
	}

	return max;
#endif
}

int main(int argc, const char *argv[])
{
	int buf[6] = {0,1,1,1,1,0};
	int num;
	num = findMaxConsecuitveOnes(buf,6);

	printf("num = %d\n",num);
	return 0;
}
