#include <stdio.h>
#include <stdlib.h>

typedef int data_t;
typedef struct dblink{
	data_t data;
	struct dblink *front;
	struct dblink *next;
}dblink_t;

dblink_t *creat_dblink()
{
	dblink_t *head;
	dblink_t *tail;

	head = malloc(sizeof(dblink_t));
	
	head->next = head;
	head->front = head;


	return head;
}


int ins_head_dblink(dblink_t *head,const data_t *data)
{
	dblink_t *newnode;
	newnode = malloc(sizeof(dblink_t));

	newnode->data = *data;

	newnode->next = head->next;
	newnode->front = head;

	head->next->front = newnode; // newnode->next->front = newnode;
	head->next = newnode;        // newnode->front->next = newnode;

	return 0;
}

int del_head_dblink(dblink_t *head)
{
	dblink_t *temp;
	
	if(head == head->next)
		return -1;

	temp = head->next;

	temp->next->front = head;//head->next->next->front = head;
	head->next = temp->next; //head->next = head->next->next;

	free(temp);
	return 0;
}

void print_dblink(dblink_t *head)
{
	dblink_t *p = head;
	dblink_t *tail;
	while(p != head->next){
		head = head->next;
		printf("%d ",head->data);
	}
	printf("\n");

	tail = head->next;
	while(p != tail->front){
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

		
	for(i = 10;i >= 0;i --){
		ins_head_dblink(head,&i);
	}

	print_dblink(head);


	return 0;
}


