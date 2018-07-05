#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int lengthOfLongestSubstring(char *s)
{
	int buf[128];
	int max = 0;
	int len = 0;
	int index = 0;

	memset(buf,0xff,sizeof(buf));

	while(*s != '\0'){
		if(buf[*s] == -1){
			len++;
		}else{
		  if(index - buf[*s] > len){
		  	len ++;
		  }else{
		  	len = index - buf[*s];
		  }
		}
		if(len > max)
		  max = len;
		buf[*s++] = index++;
	}

	return max;

}

int main(int argc, const char *argv[])
{
	int num;
	char *str = "AbcdABCDABCDE";

	printf("1\n");
	num = lengthOfLongestSubstring(str);
	printf("maxstring = %d\n",num);
	return 0;
}
