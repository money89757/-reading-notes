#include <stdio.h>

void Shellsort(char A[],int N)
{
	int i,j, Increment;
	char tmp;

	for(Increment = N/2; Increment > 0 ; Increment /= 2)
		for(i = Increment; i < N; i++)
		{
			tmp = A[i];
			for(j = i; j >= Increment; j -= Increment)
				if(tmp < A[j - Increment])
					A[j] = A[j - Increment];
				else
					break;

			A[j] = tmp;
		}
}


int main(int argc, const char *argv[])
{
	char chr[10] = {22,33,11,2,3,1,6,7,5,10};
	for(int i = 0; i < 10; i++)
	{
		printf("%d ",chr[i]);
	}

	printf("\n");

	Shellsort(chr,10);

	for(int i = 0; i < 10; i++)
	{
		printf("%d ",chr[i]);
	}
	return 0;
}
