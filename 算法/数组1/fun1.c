#include <stdio.h>

int main(int argc, const char *argv[])
{
	char a[5] = {5,4,3,2,1};
	int i = 0;

	for(i = 4;i >= 0; i--)
	{
		printf("\n%d",a[i]);
	}
	return 0;
}
