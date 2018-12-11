#include <stdio.h>
#include <stdlib.h>

int *dailyTemperatyres(int *temperatures, int temperaturesSize, int *returnSize)
{
	int n = temperaturesSize;
	int *res = (int *)malloc(sizeof(int) * n);
	memset(res, 0, sizeof(int)*n);
	int *stack = (int *)malloc(sizeof(int) * n);
	int k = 0, i = 0;
	for(i = 0; i < n; i++)
	{
		while(k != 0 && temperatures[i] > temperatures[stack[k-1]])
		{
			res[stack[k-1]] = i - stack[k-1];
			k--;
		}
		stack[k++] = i;
	}
	*returnSize = n;
	
	return res;
}

int main(int argc, const char *argv[])
{
	int temp[] = {73, 74, 75, 71, 69, 72, 76, 73};
	int returnSize[8];
	int *res = dailyTemperatyres(temp,8,returnSize);
	int i;
	for(i = 0; i < 8; i++)
	{
		printf("%d ",res[i]);
	}

	return 0;
}
