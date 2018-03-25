#include <stdio.h>

void InsertionSort(char A[],int N);

void InsertionSort(char A[], int N)
{
	int j,P;

	char Tmp;

	for(P = 1; P < N; P++)
	{
		Tmp = A[P];
		for(j = P; j > 0 && A[j - 1] > Tmp; j--)
			A[j] = A[j - 1];

		A[j] = Tmp;
	}
}


int main(int argc, const char *argv[])
{
	char str[8] = {1,4,5,8,3,4,10,15};
	for(int i = 0; i < 8; i++)
	{
		printf("%d ",str[i]);

	}

	InsertionSort(str,8);
	
	printf("\n");
	for(int i = 0; i < 8; i++)
	{
		printf("%d ",str[i]);
	}
	return 0;
}
