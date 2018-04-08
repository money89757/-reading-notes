#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if 0
int countSegments(char *s)
{
	int i;
	char c;
	int num = 0;
	int word = 0;
	for(i = 0; (c=s[i]) != '\0'; i++)
	{
		if(c < 'A'||c >'Z' && c < 'a'||c > 'z')
		word = 0;
		else if(word == 0)
		{
			word = 1;
			num++;
		}
	}

	return num;
}
#endif

#if 0

int countSegments(char* s) {
    char *p = s;
    int count = 0;
    while( *p == ' ') p++;
    while(*p != '\0'){
        count++;
        while(*p !=' ' && *p !='\0'){
            printf("%c",*p);
            p++;
            
        }
        while(*p ==' ') p++;
        
        printf("count =%d " , count) ;
    }
    return count;
}

#endif

int countSegments(char *s)
{
	char *cp,*q;
	int cnt = 0;
	for(cp = s; *cp != '\0';)
	{
		q = cp;
		while(*q != ' ' && *q != '\0')
		q++;
		if(q != cp)
		{
			cnt++;
			cp = q;
		}
		if(*q == ' ')
		{
			*q = '\0';
			cp++;
		}
	}
	return cnt;
}


int main(int argc, const char *argv[])
{
	char str[40] = "I have an app'le I have a pen";
	int c;
	c = countSegments(str);

	printf("countSegments = %d\n", c);
	return 0;
}
