#include <stdio.h>
#include <string.h>

int main()
{
    char a[101],s[101];
    int i,len,mid,next,top;

 //   a[101]="helhe";
    a[101] = "hel";
    len = strlen(a);
    mid = len/2 - 1;

    top = 0;
    for(i = 0;i <= mid; i++)
        s[++top] = a[i];
    
    if(len%2 == 0)
        next = mid + 1;
    else
        next = mid + 2;
    
    for(i = next;i <= len-1;i++)
    {
        if(a[i] != s[top])
            break;
        top--;
    }

    if(top == 0)
        printf("yes");
    else
        printf("no");

 //   getchar();
   // getchar();

    return 0;
}