

#include <stdio.h>
#include <stdlib.h>

typedef int data_t;
typedef struct tree{
	data_t data;
	struct tree *lchild;
	struct tree *rchild;
}tree_t;


tree_t *init_tree(int num,int max)
{
	tree_t *root;

	if(num > max)
		return NULL;
	root = malloc(sizeof(tree_t));
	
	root->data = num;

	root->lchild = init_tree(num * 2,max);
	root->rchild = init_tree(num * 2 + 1,max);

	return root;
}


void pre(tree_t *root)
{
	if(NULL == root)
		return ;
a1:	printf("%d ",root->data);
a2:	pre(root->lchild);
a3:	pre(root->rchild);

a4:	return ;
}


int pre_stack(tree_t *root)
{
	tree_t *stack[10];
	int top = -1;

	while(-1 != top || NULL != root){

		while(NULL != root){
			printf("%d ",root->data);
			stack[++ top] = root;
			root = root->lchild;
		}
		root = stack[top --];
		root = root->rchild;

	}

	printf("\n");

	return 0;
}



void mid(tree_t *root)
{
	if(NULL == root)
		return ;
	mid(root->lchild);
	printf("%d ",root->data);
	mid(root->rchild);

	return ;
}
void post(tree_t *root)
{
	if(NULL == root)
		return ;
	post(root->lchild);

	post(root->rchild);
	printf("%d ",root->data);

	return ;
}


int post_stack(tree_t *root)
{
	struct sss{
		tree_t *root;
		int flag;
	}*stack;
	int top = -1;
	stack = malloc(sizeof(struct sss) * 1000);

	while(NULL != root || -1 != top){

		while(NULL != root){
			stack[++ top].root = root;
			stack[ top].flag = 1;
			root = root->lchild;
		}
		if(1 == stack[top].flag){
			root = stack[top].root->rchild;
			stack[top].flag = 2;
		}else if(2 == stack[top].flag){
			printf("%d ",stack[top --].root->data);
		}

	}

	printf("\n");

	return 0;

}





int main(int argc, const char *argv[])
{
	tree_t *root;

	root = init_tree( 1 ,6);

	pre(root);

	printf("\n");

	mid(root);
	printf("\n");

	post(root);
	printf("\n");


	pre_stack(root);

	post_stack(root);
	return 0;
}
