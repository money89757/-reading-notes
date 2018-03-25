#include <stdio.h>
#include <stdlib.h>

void delspace(char *str)
{

	char *p = str;
	char *t = p;

	while(*p != '\0')
	{
		if(*p == ' ')
		{
			t++;
			if(*t != ' ')
			{
				*p = *t;
				*t = ' ';
			}
		}
		else
		{
			p++;
			t = p;
		}
	}
}

int main(int argc, const char *argv[])
{
	
	char str[1024] = { 0 };
	
	scanf("%[^\n]",str);

	delspace(str);

	printf("out = %s \n",str);
	return 0;
}
