#include <stdio.h>
#include <string.h>

int numJeweLSInStones(char *J, char *S)
{
	int i = 0;
	int j = 0;
	int s = 0;
	for(j = 0; j <strlen(J); j++)
	{
		for(s = 0; s < strlen(S); s++)
		{
			if(J[j] == S[s])
				i++;
		}
	}

	return i;
}

int main(int argc, const char *argv[])
{
	char *a = "ajcdcdcd";
	char *b = "abcdcdcdz";
	int c = numJeweLSInStones(a,b);
	printf("c = %d\n",c);
	return 0;
}
