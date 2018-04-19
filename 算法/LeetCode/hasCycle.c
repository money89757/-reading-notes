#include <stdio.h>

int hasCycle(struct ListNode *head)
{
	if(head == NULL)
		return -1;

	ListNode *fast = head;
	ListNode *slow = head;

	while(fast->next != NULL &&　fast->next->next != NULL)
	{	fast = fast->next->next;
		slow = slow->next;
		if(fast == slow)
			return 0;
	}

	return -1; 
}
