#include <stdio.h>

int main(int argc, const char *argv[])
{
	volatile int i = 10;

	int a = i;
	printf("\ni = %d",a);

	__asm
	{
		mov dword ptr[ebp-4],20h
	}

	int b = i;
	printf("\ni = %d",b);
	return 0;
}
