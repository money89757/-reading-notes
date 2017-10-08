#include <stdio.h>
#include <stdlib.h>
typedef int data_t;

typedef struct list{
	data_t *data; //保存数据的空间
	int size;   //容器能保存的元素个数
	int last; //保存最后一个有效元素的下标
}list_t;
/*返回创建好的 空顺序表 的地址*/

list_t *creat_list(int size)
{
	list_t *p;
	p = malloc(sizeof(list_t) + (sizeof(data_t) * size));//申请空间
	p->data = (data_t *)(p + 1);
	p->size = size;
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
int ins_index_list(list_t *p,int index,const data_t *data)
{

	//1.将插入位置的数据向下移动
	//  1.1 移动最后一个 到下一个位置   data[last + 1] = data[last];
	//  1.2 移动倒数第二个             
	//  1.3 。。。。
	//
	//2.写入数据到对应位置
	//3. last ++
	
	int i;
	
	if(p->size - 1 == p->last) // 如果容器满 直接退出
		return -1;
	if(index < 0 || index > p->last + 1)
		return -2;

	for(i = p->last;i >= index;i --){
		p->data[i + 1] = p->data[i];
	}

	p->data[index] = *data;

	p->last ++; // (*p).last

	return 0;
}

int del_index_list(list_t *p,int index)
{
	//1.移动删除位置之下的数据
	//   1.1 index + 1 位置的数据到 index
	//   1.2 index + 2              index + 1
	//   ....
	//2.laint main(int argc, const char *argv[])
	
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

int get_index_list(list_t *p,int index,data_t *databuf)
{
	if(-1 == p->last)  //如果为空，则退出
		return -1;
	if(index < 0 || index > p->last)
		return -2;

	*databuf = p->data[index];

	return 0;
}

int change_index_list(list_t *p,int index,const data_t *newdata)
{
	if(-1 == p->last)  //如果为空，则退出
		return -1;
	if(index < 0 || index > p->last)
		return -2;

	p->data[index] = *newdata;

	return 0;
}
/*如果为满 返回 真1；非满 返回假0*/
int is_full_list(list_t *p)
{

	return (p->size - 1) == p->last;
}

int is_empty_list(list_t *p)
{
	return -1 == p->last;
}

int locate_list(list_t *p,const data_t *data)
{
	int i;

	if(NULL == p || NULL == data)
		return -2;

	for(i = 0;i <= p->last;i ++){
		if(*data == p->data[i])
			return i;
	}

	return -1;
}

int clean_list(list_t *p)
{
	p->last = -1;
	return 0;
}

int dis_list(list_t *p)
{
	free(p);
	
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


