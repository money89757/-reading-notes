#include <stdio.h>
#include <stdlib.h>

struct ListNode {
	int val;
	struct ListNode *next;
};

struct ListNode* deleteDuplicates(struct ListNode* head) {
    
        if(head != NULL)
        {
            struct ListNode* nodeA = head;
            struct ListNode* nodeB = head->next;
            while(nodeB != NULL)
            {
                if(nodeA->val == nodeB->val){
                    nodeA->next = nodeB->next;
                    nodeB = nodeB->next;
                }else{
                    nodeA = nodeB;
                    nodeB = nodeB->next;
                }
            }
        }
        return head;
    
}

