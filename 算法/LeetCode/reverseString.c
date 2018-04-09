#include <stdio.h>
#include <string.h>

char *reverseString(char *s)
{
#if 0
	int len = 0;
	char *p = s;
	while(*p != 0)
	{
		len++;
		p++;
	}

	//chang
	int i = 0;
	char c;
	while(i <= len/2 -1)
	{
		c = *(s + i);
		*(s + i) = *(s + len - 1 -i);
		*(s + len - 1 - i) = c;
		i++;
	}

	return s;
#endif

	int start = 0;
	int end = 0;

	while(s[end] != '\0') { ++end ;}
	--end;

	while(start < end)
	{
		s[start] += s[end];
		s[end] = s[start] - s[end];
		s[start] -= s[end];

		++start;
		--end;
	}

	return s;

}

int main(int argc, const char *argv[])
{
	char s[12] = "Hello world";
	reverseString(s);
	printf("%s",s);

	return 0;
}
