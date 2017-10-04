#include <stdio.h>
#include <stdlib.h>

typedef int data_t;
typedef struct {
	data_t *data;
	int size;
	int top;
}my_stack_t;

my_stack_t *creat_stack(int size)
{
	my_stack_t *sp;
	sp = malloc(sizeof(my_stack_t ));
	sp->data = malloc(sizeof(data_t) * size);
	sp->size = size;
	sp->top = -1;

	return sp;
}
/*入栈一个元素*/
int push_stack(my_stack_t *sp,const data_t *data)
{
	if(sp->size - 1 == sp->top)
		return -1;

	sp->data[ ++ sp->top] = *data;

	return 0;
}
/*出栈一个元素*/
int pop_stack(my_stack_t *sp,data_t *databuf)
{
	if(-1 == sp->top)
		return -1;

	*databuf = sp->data[sp->top --];

	return 0;
}
/*读取栈顶元素数据*/
int get_top_stack(my_stack_t *sp,data_t *databuf)
{
	if(-1 == sp->top)
		return -1;

	*databuf = sp->data[sp->top];

	return 0;
}

int is_empty_stack(my_stack_t *sp)
{
	return -1 == sp->top;
}

int is_full_stack(my_stack_t *sp)
{
	return sp->size - 1 == sp->top;
}

int get_length_stack(my_stack_t *sp)
{
	return sp->top + 1;
}
int clean_stack(my_stack_t *sp)
{
	sp->top = -1;
	return 0;
}
int dis_stack(my_stack_t **spp)
{
	free((*spp)->data);
	free(*spp);

	return 0;
}


void print_stack(my_stack_t *sp)
{
	int i;
	for(i = sp->top;i >= 0;i --){
		printf("%d\n",sp->data[i]);
	}
	printf(":end:\n");
}


int main(int argc, const char *argv[])
{
	my_stack_t *sp;
	data_t buf;
	int i;
	sp = creat_stack(6);

	for(i = 0;i < 8;i ++){
		buf = i;
		push_stack(sp,&buf);
	}

	print_stack(sp);
	
	while(i --){
		buf = -1;
		pop_stack(sp,&buf);
		printf("pop:%d\n",buf);
	}


	return 0;
}

