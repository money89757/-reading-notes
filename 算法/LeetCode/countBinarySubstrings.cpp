#include <stdio.h>
#include <string.h>

int countBinarySubstrings(char *s)
{
	int zero = 0;
	int one = 0;
	int ret = 1;
	int string = strlen(s);
	
	for(int i = 1; i < string; ++i)
	{
		if(s[i] == s[i-1]) ++ret;
		
		else
		{
			one = ret;
			ret = 1;
		}
		
		if(one >= ret) ++zero; 
	}
	
	return zero;
}

int main(void)
{
	char ch[] = "";
	
	
	printf("%d\n",countBinarySubstrings(ch));
}
