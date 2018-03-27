#include<stdio.h>  
  
int c=0;  
  
/*heapadjust()函数的功能是实现从a[m]到a[n]的数据进行调整，使其满足大顶堆的特性*/  
/*a[]是待处理的数组，m是起始坐标， n是终止坐标*/  
void heapadjust(int a[], int m, int n)  
{  
    int i, temp;  
    temp=a[m];  
  
    for(i=2*m;i<=n;i*=2)//从m的左孩子开始  
    {  
        if(i+1<=n && a[i]<a[i+1])//如果左孩子小于右孩子，则将i++，这样i的值就是最大孩子的下标值  
        {  
            i++;  
        }  
  
        if(a[i]<temp)//如果最大的孩子小于temp，则不做任何操作，退出循环；否则交换a[m]和a[i]的值，将最大值放到a[i]处  
        {  
            break;  
        }  
        a[m]=a[i];  
        m=i;  
    }  
    a[m]=temp;  
}  
  
void crtheap(int a[], int n)//初始化创建一个大顶堆  
{  
    int i;  
    for(i=n/2; i>0; i--)//n/2为最后一个双亲节点，依次向前建立大顶堆  
    {  
        heapadjust(a, i, n);  
    }  
}  
  
/*swap()函数的作用是将a[i]和a[j]互换*/  
void swap(int a[], int i, int j)  
{  
    int temp;  
    temp=a[i];  
    a[i]=a[j];  
    a[j]=temp;  
    c++;  
}  
  
void heapsort(int a[], int n)  
{  
    int i;  
  
    crtheap(a, n);  
    for(i=n; i>1; i--)  
    {  
        swap(a, 1, i);//将第一个数，也就是从a[1]到a[i]中的最大的数，放到a[i]的位置  
        heapadjust(a, 1, i-1);//对剩下的a[1]到a[i]，再次进行堆排序，选出最大的值，放到a[1]的位置  
    }  
}  
  
int main(void)  
{  
    int i;  
    int a[100]={0};  
    printf("排序前：");  
    for(i=1;i<100;i++)  
    {  
		a[i] = rand()%100;
        printf("%d ",a[i]);  
    }  
    heapsort(a, 100);  
    printf("\n\n共交换数据%d次\n\n", c);  
    printf("排序后：");  
    for(i=1;i<100;i++)  
    {  
        printf("%d ",a[i]);  
    }  
    printf("\n\n\n");  
    return 0;  
} 
