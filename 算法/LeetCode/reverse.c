#include <stdio.h>

int reverse(int x)
{
	int org = x, result = 0;
	while(org)
	{
		int num = org % 10;
		int temp = num + result * 10;
		org /= 10;
		if(temp / 10 != result)
		{
			result = 0;
			break;
		}

		result = temp;
	}

	return result;
}

int main(int argc, const char *argv[])
{
	int a = 123;
	int b = -123;
	int c = 120;

	printf("%d \n",reverse(a));
	printf("%d \n",reverse(b));
	printf("%d \n",reverse(c));


	return 0;
}
