#include <stdio.h>


int judgeCircle(char *move, int size)
{
	int x = 0,y = 0,i = 0;
	
	for(i;i < size; i++)
	{
		if(*(move + i) == 'U') y--;
		else if (*(move + i) == 'D') y++;
		else if (*(move + i) == 'L') x--;
		else if (*(move + i) == 'R') x++;
	}
	
	return x == 0 && y == 0;
}

int main(void)
{
	char ch[] = "U";
	printf("%d\n",judgeCircle(ch, 1));
	return 0;
}
