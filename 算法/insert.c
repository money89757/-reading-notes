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
//提高插入排序：只需要在内循环中将较大的元素都向右移动而不是
//总是交换两种元素（这样访问数组的次数就能减半）

//插入排序对于部分有序的数组十分高效，也很适合小规模数组