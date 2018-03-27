#include <stdio.h>

#define LeftChild(i) (2 * (i) + 1)

void PercDown(int A[], int i, int N);
void HeapSort(int A[],int N);


void PercDown(int A[], int i, int N)
{
	int Child;
	int tmp;

	for(tmp = A[i]; LeftChild(i) < N; i = Child)
	{
		Child = LeftChild(i);
		if(Child != N - 1 && A[ Child+ 1] > A[Child])
			Child++;
		if(tmp < A[Child])
			A[i] = A[Child];
		else
			break;
	}
	A[i] = tmp;
}

void HeapSort(int A[], int N)
{
	int i;

	for(i = N/2; i >= 0; i--)
		PercDown(A, i, N);
	for(i = N-1; i > 0; i--)
	{
// 		Swap(&A[0],&A[i]);
		PercDown(A, 0, i);
	}
}

int main(int argc, const char *argv[])
{
	int str[100];
	int i;
	for(i = 0; i < 100; i++)
	{
		str[i] = rand()%100;
		printf("%d ",str[i]);
	}
	printf("\n ==\n");
	HeapSort(str,100);

	for(i = 0; i < 100; i++)
	{
		printf("%d ",str[i]);
	}
	return 0;
}
