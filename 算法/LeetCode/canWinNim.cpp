#include <stdio.h>

int canWinNim(int n)
{
	return n%4 != 0;
}

int main(void)
{
	int n = 10;
	int num = canWinNim(n);
	
	printf("num = %d\n", num);
}
