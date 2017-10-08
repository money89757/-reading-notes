#include <stdio.h>
#include <stdlib.h>

typedef int data_t;
typedef struct linknode{
	data_t data;
	struct linknode *next;
}link_t;


link_t *creat_one()
{
	link_t *p;
	p = malloc(sizeof(link_t));
	p->next = p;

	p->data = 1;

	return p;
}

int ins_head_link(link_t *p,int num)
{
	link_t *newnode;
	newnode = malloc(sizeof(link_t));
	newnode->data = num;
	newnode->next = p->next;

	p->next = newnode;

	return 0;
}
link_t *init_jos()
{
	int i;
	link_t *p;
	//1.创建一个单结点环 
	p = creat_one();
	
	//2.插入剩余的结点
	for(i = 10;i >= 2;i --){
		ins_head_link(p,i);
	}

	return p;
}

int del_head_link(link_t *head)
{
	if(head == head->next)
		return -1;
	link_t *temp;
	temp = head->next;
	head->next = temp->next;

	printf("%d\n",temp->data);
	free(temp);

	return 0;
}

link_t *jos_once(link_t *p)
{
	int i = 3 - 2;
	while(i --){
		p = p->next;
	}
	del_head_link(p);

	return p->next;
}

link_t *jos(link_t *p)
{
	//重复执行删除到剩下两个为止
	

	while(p != p->next->next){
		p = jos_once(p);
	}
	
	return p;
}

void print_clink(link_t *p)
{
	link_t *temp = p;

	do{
		printf("%d ",p->data);
		p = p->next;
	}while(p != temp);
	
	printf("\n");

	return ;
}

int main(int argc, const char *argv[])
{

	link_t *p;
	//1.创建一个环
	p = init_jos();

	print_clink(p);

	//2.执行删除
	p = jos(p);
	//3.打印
	
	print_clink(p);


	return 0;
}

