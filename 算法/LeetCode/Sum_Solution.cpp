#include <stdio.h>

int Sum_Solution(int n)
{
	if(n == 0)
		return 0;
	else
		return Sum_Solution(n-1)+n;
}

int main(void)
{
	printf("%d\n",Sum_Solution(20));
}
