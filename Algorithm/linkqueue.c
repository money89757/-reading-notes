#include <stdio.h>
#include <stdlib.h>

typedef int data_t;
typedef struct linkqueue_node{
	data_t data;
	struct linkqueue_node *next;
}linkqueue_node_t;

typedef struct linkqueue{
	linkqueue_node_t *head;
	linkqueue_node_t *tail;
}linkqueue_t;


linkqueue_t *creat_linkqueue()
{
	//1.创造一个保存头尾指针的结点
	//2.创造链表头
	linkqueue_t *p;
	p = malloc(sizeof(linkqueue_t));
	p->head = malloc(sizeof(linkqueue_node_t));
	p->head->next = NULL;
	p->tail = p->head;

	return p;
}

int in_linkqueue(linkqueue_t *lqp,const data_t *data)
{
	//1.执行尾部插入
	//2.记录 新的尾部地址
	linkqueue_node_t *newnode;
	newnode = malloc(sizeof(linkqueue_node_t));
	newnode->data = *data;
	newnode->next = NULL;//lqp->tail->next;

	lqp->tail->next = newnode;

	lqp->tail = newnode;

	return 0;
}

int out_linkqueue(linkqueue_t *lqp,data_t *databuf)
{
	//1。执行头部删除
	//2.判断是否呀恢复 尾指针的值到初始状态
	
	linkqueue_node_t *temp;

	if(NULL == lqp->head->next)
		return -1;

	temp = lqp->head->next;
	lqp->head->next = temp->next;
	
	*databuf = temp->data;

	free(temp);

	if(NULL == lqp->head->next)
		lqp->tail = lqp->head;

	return 0;
}

int get_head_linkqueue(linkqueue_t *lqp,data_t *databuf)
{
	if(NULL == lqp->head->next)
		return -1;
	*databuf = lqp->head->next->data;
	return 0;
}

void print_linkqueue(linkqueue_t *lqp)
{
	linkqueue_node_t *head;
	head = lqp->head;
	while(NULL != head->next){
		head = head->next;
		printf("%d ",head->data);
	}
	printf(":tail:\n");

	return ;
}


int main(int argc, const char *argv[])
{
	linkqueue_t *lqp;
	int i;
	data_t buf;
	lqp = creat_linkqueue();


	for(i = 0;i < 10;i++){
		in_linkqueue(lqp,&i);

	}

	print_linkqueue(lqp);
	i = 11;
	while(i --){
		buf = -1;
		out_linkqueue(lqp,&buf);
		printf("out:%d\n",buf);
	}

	return 0;
}
