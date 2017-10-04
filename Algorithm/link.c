#include <stdio.h>
#include <stdlib.h>

typedef int data_t;
typedef struct linknode{
	data_t data;//保存数据
	struct linknode *next; //保存下一个元素地址
}link_t;

link_t *creat_link(void)
{
	link_t *head;
	head = malloc(sizeof(link_t));//申请头结点空间
	head->next = NULL;// 初始化 头结点里的next指针

	return head; 
}
/*在头结点之后插入一个新的结点*/
int ins_head_link(link_t *head,const data_t *data)
{
	//1.申请空间保存数据
	//2.新的结点指向后一个结点
	//3.头接的 指向 新的结点 
	link_t *newnode;
	newnode = malloc(sizeof(link_t));
	newnode->data = *data;

	newnode->next = head->next;
	//(*newnode).next = (*head).next;
	head->next = newnode;

	return 0;
}
int del_head_link(link_t *head)
{
	//1.备份要删除结点的地址
	//2.头结点指向下一个元素
	//3.释放 删除结点
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
	//1.定位到要删除结点的前一个
	//  1.1 p = p->next 循环 index次
	//2.执行删除
	
	if(NULL == head->next ||index < 0)
		return -1;

	while(index --){
		head = head->next;
		if(NULL == head->next)
			return -1;
	}
	del_head_link(head);
	
	return 0;
}
int ins_index_link(link_t *head,
		int index,const data_t *data)
{
	//1.定位 
	
	//2.执行插入
	
	while(index --){
		head = head->next;
		if(NULL == head)
			return -1;
	}

	ins_head_link(head,data);

	return 0;
}

int change_head_link(link_t *head,const data_t *newdata)
{
	if(NULL == head->next)
		return -1;
	head->next->data = *newdata;

	return 0;
}
int change_index_link(link_t *head,int index,const data_t *newdata)
{
		
	if(NULL == head->next ||index < 0)
		return -1;

	while(index --){
		head = head->next;
		if(NULL == head->next)
			return -1;
	}
	change_head_link(head,newdata);

	return 0;
}

int get_length_link(link_t *head)
{
	int count = 0;
	while(NULL != head->next){
		head = head->next;
		count ++;
	}

	return count;
}

int get_head_link(link_t *head,data_t *databuf)
{
	if(NULL == head->next)
		return -1;
	*databuf = head->next->data;

	return 0;
}
int get_index_link(link_t *head,int index,data_t *databuf)
{
	if(NULL == head->next ||index < 0)
		return -1;

	while(index --){
		head = head->next;
		if(NULL == head->next)
			return -1;
	}

	get_head_link(head,databuf);

	return 0;
}

int empty_link(link_t *head)
{
	return NULL == head->next;
}
int clean_link(link_t *head)
{
	while(0 == del_head_link(head))
		;
	return 0;
}

int dis_link(link_t **headp)
{
	clean_link(*headp);
	free(*headp);
	*headp = NULL;

	return 0;
}

link_t * locate_link(link_t *head,const data_t *data)
{

	while(NULL != head->next){
		if(*data == head->next->data)
			return head;
		head = head->next;
	}

	return NULL;
}

int del_locate_link(link_t *head,const data_t *data,const data_t *newdata)
{
	head = locate_link(head,data);
	if(NULL == head)
		return -1;
	return  del_head_link(head);
}
void print_link(link_t *head)
{
	link_t *p;
	p = head->next;
	while(p != NULL){//直到获取到NULL结束
		printf("%d ",p->data);//打印元素数据
		p = p->next;//获取下一个元素地址
	}
	printf("\n");
	return ;
}
int main(int argc, const char *argv[])
{
	link_t *head;
	link_t *p;
	int i;
	head = creat_link();
	i = 290;
	ins_index_link(head,1,&i);

	for(i = 7;i >= 0;i --){
		ins_head_link(head,&i);
	}

	print_link(head);
	//del_index_link(head,20);
	i = 250;
	ins_index_link(head,9,&i);
	print_link(head);

	i = 5;
	
	p = locate_link(head,&i);
	printf(">>%d\n",p->next->data);

	i = 4;
	p = locate_link(head,&i);
	i = 123;
	change_head_link(p,&i);
	print_link(head);
	dis_link(&head);
	return 0;
}
