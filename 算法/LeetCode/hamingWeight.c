#include <stdio.h>
#include <stdint.h>
int hamingWeight(uint32_t n)
{
	int count = 0;
	while(n)
	{
		if(n&0x01)
			++count;
		n = (n >> 1);
	}

	return count;
}

int main(int argc, const char *argv[])
{
	int num;
	scanf("%d",&num);
	int ret = hamingWeight(num);
	printf("\n%d ",ret);
	return 0;
}
