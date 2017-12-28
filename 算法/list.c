#include <stdio.h>
#include <stdlib.h>

#define SIZE 8
typedef int data_t;
typedef struct list{
	data_t data[SIZE]; //保存数据的空间
	int last; //保存最后一个有效元素的下标
}list_t;

/*返回创建好的 空顺序表 的地址*/

list_t *creat_list()
{
	list_t *p;
	p = malloc(sizeof(list_t));//申请空间
	p->last = -1;  //初始化 结构内部相应内容

	return p;
}
/*
 * p： 顺序表的地址
 * index：插入的下标位置
 * data：插入的数据
 *
 * return：成功返回0 ；失败返回-1
 * */
int ins_index_list(list_t *p,int index,data_t data)
{

	//1.将插入位置的数据向下移动
	//  1.1 移动最后一个 到下一个位置   data[last + 1] = data[last];
	//  1.2 移动倒数第二个             
	//  1.3 。。。。
	//
	//2.写入数据到对应位置
	//3. last ++
	
	int i;
	
	if(SIZE - 1 == p->last) // 如果容器满 直接退出
		return -1;
	if(index < 0 || index > p->last + 1)
		return -2;

	for(i = p->last;i >= index;i --){
		p->data[i + 1] = p->data[i];
	}

	p->data[index] = data;

	p->last ++; // (*p).last

	return 0;
}

int del_index_list(list_t *p,int index)
{
	//1.移动删除位置之下的数据
	//   1.1 index + 1 位置的数据到 index
	//   1.2 index + 2              index + 1
	//   ....
	//2.last --
	
	int i;
	if(-1 == p->last)  //如果为空，则退出
		return -1;
	if(index < 0 || index > p->last)
		return -2;

	for(i = index;i < p->last;i ++){
		p->data[i] = p->data[i + 1];
	}

	p->last --; //p->last // (*p).last
	
	return 0;
}


void print_list(list_t *p)
{
	int i;
	for(i = 0;i <= p->last;i ++){// 打印数据集合里的内容
		printf("%d ",p->data[i]);
	}
	printf("\n");
	printf("last:%d\n",p->last);//打印
								//最后一个元素位置
	return ;
}

int main(int argc, const char *argv[])
{
	list_t *p;
	list_t *p1;

	int i;

	p = creat_list();
	p1 = creat_list();

	for(i = 6;i >= 0;i --){
		ins_index_list(p,0,i);
	}

	print_list(p);

	del_index_list(p,7);
	print_list(p);

	return 0;
}



