#include <stdio.h>

bool isBadVersion(int version);

int firstBadVersion(int n)
{
	int mid,left = 1,right = n;

	while(left < right)
	{
		mid = left + ((right-left)>>1);
		isBadVersion(mid) ? (right = mid) : (left = mid + 1);
	}

	return right;
}
