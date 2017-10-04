#include <stdio.h>
#include <stdlib.h>

typedef int data_t;
typedef struct dblink_node{
	data_t data;
	struct dblink_node *next;
	struct dblink_node *front;
}dblink_t;

dblink_t *creat_dblink()
{
	dblink_t *head;
	dblink_t *tail;
	head = malloc(sizeof(dblink_t));
	tail = malloc(sizeof(dblink_t));

	head->front = NULL;
	head->next = tail;

	tail->front = head;
	tail->next = NULL;

	return head;
}

int ins_head_dblink(dblink_t *head,const data_t *data)
{
	dblink_t *newnode;
	newnode = malloc(sizeof(dblink_t));

	newnode->data = *data;
	newnode->front = head;
	newnode->next = head->next;

	head->next->front = newnode;// newnode->front->next = newnode;
	head->next = newnode; // newnode->next->front = newnode;

	return 0;
}

int del_head_dblink(dblink_t *head)
{
	dblink_t *temp;
	if(NULL == head->next->next)
		return -1;

	temp = head->next;

	head->next->next->front = head;
	head->next = head->next->next;

	free(temp);

	return 0;
}

void print_link(dblink_t *head)
{
	dblink_t *tail;
	while(head->next->next != NULL){
		head = head->next;
		printf("%d ",head->data);
	}
	printf("\n");

	tail = head->next;

	while(NULL != tail->front->front){
		tail = tail->front;
		printf("%d ",tail->data);
	}
	printf("\n");

	return ;
}

int main(int argc, const char *argv[])
{
	dblink_t *head;
	int i;
	head = creat_dblink();

	for(i = 7;i >= 0;i --){
		ins_head_dblink(head,&i);
	}

	print_link(head);

	del_head_dblink(head);
	del_head_dblink(head);
	del_head_dblink(head);
	del_head_dblink(head);
	del_head_dblink(head);
	print_link(head);
	i = 250;
	ins_head_dblink(head,&i);
	ins_head_dblink(head,&i);
	ins_head_dblink(head,&i);
	print_link(head);

	return 0;
}
