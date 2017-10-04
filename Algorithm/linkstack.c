#include <stdio.h>
#include <stdlib.h>

typedef int data_t;
typedef struct stack_node{
	data_t data;
	struct stack_node *next;
}link_stack_t;

link_stack_t *creat_linkstack()
{
	link_stack_t *head;
	head = malloc(sizeof(link_stack_t) );
	head->next = NULL;

	return head;
}


int push_linkstack(link_stack_t *lsp,const data_t *data)
{
	link_stack_t *newnode;
	newnode = malloc(sizeof(link_stack_t));
	newnode->data = *data;
	newnode->next = lsp->next;

	lsp->next = newnode;

	return 0;
}
int pop_linkstack(link_stack_t *lsp,data_t *databuf)
{
	link_stack_t *temp;
	
	if(NULL == lsp->next)
		return -1;

	temp = lsp->next;

	lsp->next = temp->next;

	*databuf = temp->data;

	free(temp);

	return 0;
}


int get_top_linkstack(link_stack_t *lsp,data_t *databuf)
{
	if(NULL == lsp->next)
		return -1;
	*databuf = lsp->next->data;
	return 0;
}
void print_linkstack(link_stack_t *lsp)
{
	while(NULL != lsp->next){
		lsp = lsp->next;
		printf("%d\n",lsp->data);
	}
	printf(":end:\n");
	
	return ;
}


int main(int argc, const char *argv[])
{

	link_stack_t *lsp;
	int i;
	data_t buf;
	lsp = creat_linkstack();

	for(i = 0;i < 6;i ++){
		push_linkstack(lsp,&i);
	}
	print_linkstack(lsp);


	i = 10;
	while(i --){
		buf = -1;
		pop_linkstack(lsp,&buf);
		printf("pop:%d\n",buf);
	}


	return 0;
}
