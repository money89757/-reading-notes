#include <stdio.h>
 
/**Definition for a binary tree node.
 * struct TreeNode{
 *    int val;
 *    struct TreeNode *left;
 *    struct TreeNode *right;
 * };
 * 
 */
   

 int maxDepth(struct TreeNode *root)
{
	int(root == NULL)
		return 0;

	int left = maxDepth(root->left);
	int right = maxDepth(root->right);

	return left > right ? (left + 1) : (right + 1);
}


