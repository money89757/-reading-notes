#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

char *addBinary(char *a, char *b)
{
	int len1 = strlen(a);
	int len2 = strlen(b);
	
	int len = len1 > len2 ? len1 + 1 : len2 + 1;
	char *result = (char *)malloc(len + 1);
	result[len] = '\0';
	result[len - 1] = '\0';
	
	int i,j,carry = 0;
	len = 0;
	
	for(i = len1 - 1, j = len2 - 1; carry || i >= 0 || j >= 0; i--,j--)
	{
		int na = i >= 0 ? a[i] - '0' : 0;
		int nb = j >= 0 ? b[j] - '0' : 0;
		result[len++] = (na ^ nb ^ carry) + '0';
		carry = carry + na + nb >= 2 ? 1 : 0;
	}

	for(i = 0, j = len - 1; i < j; i++, j--)
	{
		char c = result[i];
		
		result[i] = result[j];
		result[j] = c;
	}	
	
	return result;
}

int main(void)
{
	char a[] = "1010";
	char b[] = "1011";
	
	printf("%s\n",addBinary(a,b));
}



