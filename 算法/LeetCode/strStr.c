#include <stdio.h>
#include <strings.h>
#include <string.h>


int strStr(char *haystack, char *needle)
{
	if(haystack == NULL || needle == NULL)
		return 0;

	if(haystack == "" || needle == "")
		return 0;

	int flag = 1;
	int i,j,p;
	int haystacksize = strlen(haystack);
	int needlesize = strlen(needle);

	if(haystacksize < needlesize)
		return -1;
	for(i = 0; i < haystacksize - needlesize + 1; ++i)
	{
		p = i;
		for(j = 0; j < needlesize; ++j)
		{
			if(haystack[p++] != needle[j])
			{
				flag = 0;
				break;
			}
			flag = 1;
		}

		if(flag == 1)
			return i;

		else 
			return -1;
	}
}
int main(int argc, const char *argv[])
{
	char haystack[] = "aaaaa";
	char needle[] = "a";
	int num = strStr(haystack,needle);
	printf("strStr = %d\n",num);
	return 0;
}
