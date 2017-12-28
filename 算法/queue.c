#include <stdio.h>
#include <stdlib.h>


typedef int data_t;
typedef struct {
	data_t *data;
	int size;
	int head;// front
	int tail; // rear
}queue_t;


queue_t *creat_queue(int size) //传入存储数据的个数
{
	queue_t *qp;
	qp = malloc(sizeof(queue_t));
	qp->data = malloc(sizeof(data_t) * (size + 1) );
	qp->size = size + 1; // 数组的大小
	qp->head = 0;
	qp->tail = 0;

	return qp;
}


int in_queue(queue_t *qp,const data_t *data)
{
	if(qp->head == (qp->tail + 1) % qp->size)
		return -1;
	qp->data[qp->tail] = *data;
	qp->tail = (qp->tail + 1) % qp->size;
	return 0;
}

int out_queue(queue_t *qp,data_t *databuf)
{
	if(qp->head == qp->tail)
		return -1;

	*databuf = qp->data[qp->head];
	qp->head = (qp->head + 1) % qp->size;

	return 0;
}

void print_queue(queue_t *qp)
{
	int i;

	printf("head:");
	for(i = qp->head;i != qp->tail; i = (i + 1) % qp->size){
		printf("%d ",qp->data[i]);
	}

	printf(":tail\n");
	return ;
}

int main(int argc, const char *argv[])
{

	queue_t *qp;
	int i;
	data_t buf;
	qp = creat_queue(8);

	for(i = 0;i < 10;i ++){
		in_queue(qp,&i);
	}

	print_queue(qp);

	i = 4;
	while(i --){
		buf = -1;
		out_queue(qp,&buf);
		printf("out:%d\n",buf);
	}
	print_queue(qp);
	for(i = 10;i < 20;i ++){
		in_queue(qp,&i);
	}
	print_queue(qp);


	i = 9;
	while(i --){
		buf = -1;
		out_queue(qp,&buf);
		printf("out:%d\n",buf);
	}
	print_queue(qp);

	
	return 0;
}
