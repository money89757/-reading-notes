#include <stdio.h>

int isPalindrome(int x) {
    
    int a = x, h = 1;
  
    if (a < 0) 
        return 0;
    
    while (a / h >= 10) 
    {
        h = h * 10;
    }

    while (a > 0) 
    {
        if (a / h != a % 10) 
			return 0;
        a = a % h;
        a = a / 10;
        h = h / 100;
    }

    return 1;

}

int main(void)
{
	int i = 234432;
	int num = isPalindrome(i);
		
	printf("%d \n",num);
	
}
