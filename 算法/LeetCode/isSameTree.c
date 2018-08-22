#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct TreeNode{
	int val;
	struct TreeNode *left;
	struct TreeNode *right;
};

int isSameTree(struct TreeNode *p, struct TreeNode *q)
{
	if(p != NULL && q != NULL)
	{
		if(p->val != p->val)
		{
			return 0;
		}
		if(!isSameTree(p->left,q->left))
			return 0;
		if(!isSameTree(p->right,p->right))
			return 0;
	}
	else
		return p == q;

	return 1;
}

