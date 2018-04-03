#include <stdio.h>
#include <stdbool.h>

bool isPowerOfTwo(int n)
{
#if 0
	int count = 0;
	while(n)
	{
		n = n &(n - 1);
		count++;
	}
	
	return count;
#endif

	if(n <= 0)
		return false;
	return (n|(n-1)) == (2*n-1); 
}

int main(int argc, char const *argv[])
{
	int a,ret;
	scanf("%d",&a);
	ret = isPowerOfTwo(a);
	printf("ret=%d\n",a);
	return 0;
}


