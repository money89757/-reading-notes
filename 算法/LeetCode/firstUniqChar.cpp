#include <stdio.h>
#include <string.h>

int firstUniqChar(char *s)
{
	int str[256];
	int i;
	int l = strlen(s);
	int ret = -1;
    memset(str,0,sizeof(int)*256);  
	
	if(l)
	{
		for(i = 0; i < l; i++)
		{
			str[s[i]]++;
		}
		
		for(i = 0; i < l; i++)
		{
			if(str[s[i]] == 1)
			{
				ret = i;
				break;
			}
		}
	}	
	
	return ret;
}

int main()
{
	char *s = "lel";
	
	int i = firstUniqChar(s);
	
	printf("firstUniqChar = %d\n",i);
}
