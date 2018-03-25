#include <stdio.h>
#include <stdlib.h>


void eat(char *str)
{
	int i = 0;
	int j = 0;

	while(str[i] != '\0')
	{
		str[i] = str[j];
		if(str[i] != ' ')
		{
			i++;
		}
		j++;
	}
}


void space(char *str)
{
	char *p = str;
	char *q = str;

	while(*p = '\0')
	{
		*p = *q;
		if(*p != ' ')
		{
			*p++;
		}
		*q++;
	}
}


int main(int argc, const char *argv[])
{
	
	char str[1024] = { 0 };

	scanf("%[^\n]", str);

/***

  scanf()函数接收输入数据时，遇以下情况结束一个数据的输入：（不是结束该scanf函数，scanf函数仅在每一个数据域均有数据，并按回车后结束）。
        1.遇空格、“回车”、“跳格”键。
        2.遇宽度结束。
        3.遇非法输入。

***/

	eat(str);

//	space(str);
	printf("out : %s\n", str);
	return 0;
}
