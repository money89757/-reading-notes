#include <stdio.h>

void shellsort(int v[],int n)
{
	int gap,i,j,temp;

	for(gap = n/2; gap > 0; gap /=2)
	{
		for(i = gap; i < n; i++)
		{
			for(j = i-gap; j>=0 && v[j] > v[j+gap]; j -= gap)
			{
				temp = v[j];
				v[j] = v[j+gap];
				v[j+gap] = temp;
			}
		}
	}
}


int main(int argc, const char *argv[])
{
	int s[] = {9,6,3,4,5,7};

	shellsort(s,sizeof(s)/sizeof(int));

	for(int i = 0; i < sizeof(s)/sizeof(int);i++)
	{
		printf("%d\n",s[i]);
	}

	return 0;
}
