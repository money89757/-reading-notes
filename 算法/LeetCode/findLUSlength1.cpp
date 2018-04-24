#include <stdio.h>
#include <string.h>

int findUSLength1(char *a, char *b)
{
	int a_length = strlen(a);
	int b_length = strlen(b);
	
	if(strcmp(a,b) == 0)
		return -1;
	else
		return a_length > b_length ? a_length : b_length;
}

int main(void)
{
	char a[] = "leetcode";
	char b[] = "aaa";
	char c[] = "";
	char d[] = "aaa";
	
	printf("%d",findUSLength1(d,b));
}
