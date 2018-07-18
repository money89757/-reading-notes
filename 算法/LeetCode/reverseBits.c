#include <stdio.h>

int binaryRecursion(int n)
{
	int a;
	a = n % 2;
	n = n >> 1;
	if(n == 0);
	else
		binaryRecursion(n);
	printf("binaryRecursion = %d\n",a);
}

int reverseBits(int n)
{
	int ans = 0;
	int i;
	for(i = 0; i < 32; i++)
	{
		ans <<= 1;
		if((n & 1) == 1)
			ans++;
		n >>= 1;
	}
	return ans;
}

int main(int argc, const char *argv[])
{
	int num = 987654321;
	int i;
	binaryRecursion(num);
	i = reverseBits(num);

	printf("reverseBits = %d\n",i);
	return 0;
}
