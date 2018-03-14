#include<stdio.h>  
#include<stdlib.h>  
#define N 1024  
  
void search1(int a[N], int num)//二分法插值查找  
{  
    int tou = 0;  
    int wei = N - 1;  
    int zhong;  
    int flag = -1;//假定一开始找不到  
    int ci = 0;  
    while (tou <= wei)  
    {  
        zhong = (tou + wei) / 2;  
        //zhong=tou+(wei-tou)*(1/2)  
        printf("\n%d %d %d %d", tou, wei, zhong, ++ci);  
        if (num == a[zhong])  
        {  
            printf("Find! a[%d]=%d", zhong, num);  
            flag = 1;  
            break;  
        }  
        else if (num > a[zhong])  
        {  
            tou = zhong + 1;  
  
        }  
        else  
        {  
            wei = zhong - 1;  
        }  
    }  
    if (flag == -1)  
    {  
  
        printf("Not Find!");  
    }  
  
}  
  
void search2(int a[N], int num)//拉格朗日查找  
{  
    int tou = 0;  
    int wei = N - 1;  
    int zhong;  
    int flag = -1;//假定一开始找不到  
    int ci = 0;  
    while (tou <= wei)  
    {  
        //zhong = (tou + wei) / 2;  
        zhong = tou + (wei - tou)*1.0*(num-a[tou])/(a[wei]-a[tou]);//*1.0 乘以一个实数 避免产生误差  
        printf("\n%d %d %d %d", tou, wei, zhong, ++ci);  
        if (num == a[zhong])  
        {  
            printf("Find! a[%d]=%d", zhong, num);  
            flag = 1;  
            break;  
        }  
        else if (num > a[zhong])  
        {  
            tou = zhong + 1;  
  
        }  
        else  
        {  
            wei = zhong - 1;  
        }  
    }  
    if (flag == -1)  
    {  
  
        printf("Not Find!");  
    }  
  
}  
  
  
void main()  
{  
    int a[N];  
    for (int i = 0; i < 1024; i++)  
    {  
        a[i] = i;  
        //printf("%d  ", a[i]);  
    }  
    int num;  
    scanf("%d", &num);  
    search2(a, num);//调用函数查找  
    system("pause");  
}  