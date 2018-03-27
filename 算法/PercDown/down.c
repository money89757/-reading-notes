#include <stdio.h>

int c = 0;

void heapadjust(int a[], int m, int n)
{
	int i,temp;
	temp = a[m];

	for(i = 2*m; i <= n; i *= 2)
	{
		if(i+1 <= n && a[i] < a[i+1])
		{
			i++;
		}

		if(a[i] < temp)
		{
			break;
		}
		a[m] = a[i];
		m = i;
	}
	a[m] = temp;
}

void crtheap(int a[],int n)
{
	int i;
	for(i = n/2; i>0; i--)
	{
		heapadjust(a, i, n);
	}
}

void swap(int a[], int i, int j)
{
	int temp;
	temp = a[i];
	a[i] = a[j];
	a[j] = temp;
	c++;
}

void Heapsort(int a[], int n)
{
	int i;

	crtheap(a, n);
	for(i = n; i > 1; i++)
	{
		swap(a,1,i);
		heapadjust(a, 1, i-1);
	}
}

int main(int argc, const char *argv[])
{
	int i;
	int a[10] = {1,5,2,6,0,3,9,1,7,4};

	printf("before===\n");
	for(i = 0; i < 10; i++)
	{
		printf("/n");
		printf("%d ",a[i]);
	}
	Heapsort(a,9);
	printf("number = %d\n",c);

	printf("after===\n");
	for(i = 0; i < 10; i++)
	{
		printf("%d ",a[i]);
	}
	return 0;
}
