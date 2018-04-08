#include <stdio.h>

int hammingDistance(int x, int y)
{
	int z = x^y;
	int count = 0;
	while(z)
	{
		++count;
		z &= z-1;
	}

	return count;


#if 0

int hammingDistance(int x, int y) {
    int count=0;
    int i=0,j;
    int *a=(int *)malloc(32*sizeof(int));
    int *b=(int *)malloc(32*sizeof(int));
    while(a[i++]=x%2,x=x/2);
    i=0;
    while(b[i++]=y%2,y=y/2);
    for(j=0;j<32;j++)
    {
        if(a[j]!=b[j])
            count++;
    }
        return count;
}


#endif



int main(int argc, const char *argv[])
{
	int i = 4;
	int j = 2;
	int k;
	k = hammingDistance(i,j);
	printf("hammingDistance = %d\n",k);
	return 0;
}
