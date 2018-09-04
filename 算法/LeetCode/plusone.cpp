#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int *plusOne(int *digits, int digitsSize, int *returnSize)
{
#if 0
	int i,j,len = 0,carry = 1;
	
	int *result = (int *)malloc((digitsSize + 1) * sizeof(int));
	
	for(i = digitsSize - 1; i >= 0 || carry; i++)
	{
		int n = digits[i] + carry;
		result[len++] = n % 10;
		carry = n / 10;
	}
	
	for(i = 0, j = len - 1;i < j; i++, j--)
	{
		int tmp = result[i];
		result[i] = result[j];
		result[j] = tmp;
	}
	
	*returnSize = len;
	return result;
#endif
	
	int flag = 1;
	int i;
	int *res = NULL;
	*returnSize = digitsSize;
	for(i = digitsSize - 1; i >= 0; i--)
	{
		digits[i] = digits[i] + 1;
		flag = 0;
		if(digits[i] >= 10)
		{
			digits[i] = digits[i] - 10;
			flag = 1;
		}
		else
		break;
	}
	if(flag == 1)
	{
		res = malloc(sizeof(int) * (digitsSize + 1));
		for(i = digitsSize; i >= 0; i--)
		res[i+1] = digits[i];
		res[0] = flag;
		*returnSize = digitsSize + 1;
		return res;
	}
	
	return digits;
}

int main(int argc, char **argv) 
{
	    if (argc != 2) {
        fprintf(stderr, "Usage: ./test num\n");
        exit(-1);
    }

    int i, count = strlen(argv[1]);
    int *digits = (int *)malloc(count * sizeof(int));
    for (i = 0; i < count; i++) {
        digits[i] = argv[1][i] - '0';
    }

    int len = 0;
    int *res = plusOne(digits, count, &len);
    for (i = 0; i < len; i++) {
        printf("%c", res[i] + '0');
    }
    printf("\n");
    return 0;
}
