#ifndef __MY_TYPE_H__
#define __MY_TYPE_H__


typedef int data_t;

typedef struct list{
	data_t *data; //保存数据的空间
	int size;   //容器能保存的元素个数
	int last; //保存最后一个有效元素的下标
}list_t;


#endif
