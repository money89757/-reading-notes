#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int myAtoi(char* str)
{
    char *s;
    int n = 0, sign = 0;

    while (*str == ' ' || *str == '\t') {
        str++;
    }

    for (s = str; *s != '\0'; s++) {
        if (isdigit(*s)) {
            int d = *s - '0';
            if (sign) {
                if (-n < (INT_MIN + d) / 10) {
                    n = INT_MIN;
                    break;
                }
            } else {
                if (n > (INT_MAX - d) / 10) {
                    n = INT_MAX;
                    break;
                }
            }
            n = n * 10 + d;
        } else if (*s == '-' && isdigit(*(s + 1))) {
            sign = 1;
        } else if (*s == '+' && isdigit(*(s + 1))) {
            sign = 0;
        } else {
            break;
        }
    }

    return sign ? -n : n;   
}

int main(int argc, char **argv)
{
	char *num = "12";
	int n = myAtoi(num);
    printf("num = %x\n", n);
	
    return 0;
}
