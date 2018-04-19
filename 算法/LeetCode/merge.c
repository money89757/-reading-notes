#include <stdio.h>
#include <stdlib.h>

void merge(int *nums1, int m, int *nums2, int n)
{
	int *tmp = (int *)malloc(sizeof(int) * ( m+n ));
	int nums1Index = 0;
	int nums2Index = 0;
	int tmpIndex = 0;

	while(nums1Index < m && nums2Index < n)
	{
		if(nums1[nums1Index] < nums2[nums2Index])
		{
			tmp[tmpIndex++] = nums1[nums1Index++];
		}
		else
		{
			tmp[tmpIndex++] = nums2[nums2Index++];
		}
	}

	int i;
	if(nums1Index == m)
	{
		for(i = nums2Index; i < n; i++)
			tmp[tmpIndex++] = nums2[i];
	}
	else if(nums2Index == n)
	{
		for(i = nums1Index; i < m; i++)
			tmp[tmpIndex++] = nums1[i];
	}

	for(i = 0; i < m+n; i++)
	{	
		nums1[i] = tmp[i];
		printf("%d ",nums1[i]);
	}

	free(tmp);
}

int main(int argc, const char *argv[])
{
	int nums1[6] = {1,2,3,1,1,1};
	int nums2[3] = {2,5,6};
	int m = 6;
	int n = 3;

	merge(nums1,m,nums2,n);

	return 0;
}
