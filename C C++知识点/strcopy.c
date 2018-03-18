#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[])
{
	/* 交换前【排序 */
	char str[10][20] = {"apple","google","amd","intel","ailibaba","xiao","nike","addies","h","z","o"};
	int i,j;
	for(i = 0;i < 10; i++)
	{
		printf("%s\n", str);
	}
	for (i = 0; i < 9 - 1; i++)
	{
		/* 冒泡排序 */
		for (j = i + 1; i < 10; j++)
		{
			/* code */
			if(strcmp(str[i],str[j]) > 0)
			{
				char temp;
				//	交互字符串不该用赋值法
				strcpy(temp,str[i]);
				strcpy(str[i],str[j]);
				strcpy(str[j],temp);
			}
		}
	}

	for (i = 0; i < 10; i++)
	{
		/* 交换后字符串排序*/
		printf("%s\n",str);
	}

	return 0;
}