#include <stdio.h>

int lengthOfLastWord(char *s)
{
	int len = 0;
	while(*s != '\0')
	{
		if(s[-1] == ' ' && s[0] != ' ')
		{
			len = 1;
		}
		
		else if(*s != ' ')
		{
			len++;
		}
		
		s++;
	}
	
	return len;
}

int main(void)
{
	char str[] = "Hello world";
	int len = lengthOfLastWord(str);
	printf("%d\n",len);
	
}
