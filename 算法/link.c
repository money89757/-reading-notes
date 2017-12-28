#include <stdio.h>
#include <stdlib.h>

typedef int data_t;

typedef struct linknode{
	data_t data;
	struct linknode *next;
}link_t;

link_t *creat_link()
{
	link_t *head;
	head = malloc(sizeof(link_t));
	head->next = NULL;

	return head;
}

int ins_head_link(link_t *head,const data_t *data)
{
	//1.创建空间，保存数据
	//2.保存 后一个元素地址
	//3.头结点里保存新的元素的地址
	
	link_t *newnode;
	newnode = malloc(sizeof(link_t));
	newnode->data = *data;

	newnode->next = head->next;

	head->next = newnode;

	return 0;
}
int ins_index_link(link_t *head,int index,const data_t *data)
{
	//1.定位到要插入位置的前一个结点
	//2.执行插入
#if 0
	link_t *p;
	int count;
	p = head;
	count = 0;
	while(NULL != p->next){
		count ++;
		p = p->next;
	}

	if(index < 0 || index > count)
		return -1;
#endif

	if(index < 0)
		return -2;

	while(index --){
		head = head->next;
		if(NULL == head)
			return -1;
	}

	ins_head_link(head,data);

	return 0;
}
int del_head_link(link_t *head)
{
	link_t *temp;
	if(NULL == head->next)
		return -1;

	temp = head->next;

	head->next = temp->next;

	free(temp);

	return 0;
}

int del_index_link(link_t *head,int index)
{
	//1.定位 
	//2.删除
	
	if(NULL == head->next)
		return -1;

	while(index --){
		head = head->next;
		if(NULL == head->next)
			return -1;
	}

	del_head_link(head);
	
	return 0;
}

int change_index_link(link_t *head,int index,const data_t *newdata)
{
	//1.定位 
	//2.改
	
	if(NULL == head->next)
		return -1;

	while(index --){
		head = head->next;
		if(NULL == head->next)
			return -1;
	}
	
	head->next->data = *newdata;

	return 0;
}
/*返回找到的元素的前一个结点的地址*/
link_t *locate_link(link_t *head,const data_t *data)
{
	while(NULL != head->next){
		if(*data == head->next->data)
			return head;
		head = head->next;
	}

	return NULL;
}

int is_empty_link(link_t *head)
{
	return NULL == head->next;
}

int clean_link(link_t *head)
{
#if 0
	while(!is_empty_link(head)){
		del_head_link(head);
	}
#else
	
	while(0 == del_head_link(head));

#endif
	return ;
}

int del_locate_link(link_t *head,const data_t *data)
{
	head = locate_link(head,data);
	
	if(NULL != head){
		del_head_link(head);
		return 0;
	}

	return -1;
}


int dis_link(link_t *head)
{
	clean_link(head);
	free(head);
	return 0;
}


void print_link(link_t *head)
{
	link_t *p;
	p = head;
	while(NULL != p->next){
		p = p->next;
		printf("%d ",p->data);
	}
	printf("\n");
	return ;
}




int main(int argc, const char *argv[])
{

	link_t *p;
	link_t *node;
	int i;

	p = creat_link();

	del_index_link(p,1);
	for(i = 20;i >= 10;i --){
		ins_head_link(p,&i);
	}

	print_link(p);


	i = 250;
	ins_index_link(p,10,&i);

	print_link(p);

	del_index_link(p,13);
	print_link(p);
	
	i = 251;
	change_index_link(p,10,&i);
	print_link(p);

	i =13;
	node = locate_link(p,&i);
	printf("find:%d\n",node->next->data);
	del_head_link(node);
	print_link(p);


	clean_link(p);
	print_link(p);


	return 0;
}

