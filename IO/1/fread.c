#include<stdio.h>

int main(int argc, const char *argv[])
{
	FILE *fp;
	fp = fopen("1.txt","r");

	int a[2];
	size_t n;
//	n = fread(a,4,2,fp);
	n = fread(a,1,8,fp);
	printf("n = %d\n",n);
	printf("%x,%x\n",a[0],a[1]);

	fclose(fp);
	return 0;
}
