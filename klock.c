#include "klock.h"
#include <stdio.h>

unsigned long locknumber = 0; 
List *list = NULL;

 
List * AddList(List * list, unsigned long id1, unsigned long id2, char type)
{ 
	if (list == NULL)
	{
		size = 0;
		list = malloc(sizeof(List) * 5);
	}
	 
	if (type == T2L)
	{
		for (int i = 0; i < size; i++)
		{
			if (list[i].node.id == id1 && list[i].node.type == THREADTYPE)
			{ 
				InsertNode(&(list[i].node), id2, LOCKTYPE);
				return list;
			}
		} 
	}
	else
	{
		for (int i = 0; i < size; i++)
		{
			if (list[i].node.id == id1 && list[i].node.type == LOCKTYPE)
			{
				InsertNode(&(list[i].node), id2, T2L);
				return list;
			}
		}
	}
	list[size].node.id = id1;
	list[size].node.next = NULL;
	if (type == T2L)
	{
		list[size].node.type = THREADTYPE;
		InsertNode(&(list[size].node), id2, LOCKTYPE);
	}
	else
	{
		list[size].node.type = LOCKTYPE;
		InsertNode(&(list[size].node), id2, THREADTYPE);
	}
	size++;
	return list;
}


 
char HasCircle(List * list, unsigned long nodeid, char nodetype, unsigned long inid, char type)
{
	for (int i = 0; i < size; i++)
	{
		if (list[i].node.id == nodeid && list[i].node.type == nodetype && list[i].check == 0)
		{
			 
			list[i].check = 1;
			 
			Node* tmp = list[i].node.next;
			while (tmp != NULL)
			{
				if (tmp->id == inid && tmp->type == type)
					return 1;
				else
				{
					char flag = HasCircle(list, tmp->id, tmp->type, inid, type);
					if (flag == 1)
						return 1;
				}
				tmp = tmp->next;
			} 
			return 0;
		}
	} 
	return 0;

}
 
int CheckCircle(List * list, unsigned long id, char type)
{ 
	for (int i = 0; i < size; i++)
		list[i].check = 0;
	return HasCircle(list, id, type, id, type);
}


 
void DeleteList(List * list, unsigned long id1, unsigned long id2, char type)
{
	if (list == NULL)
		return;
	for (int i = 0; i < size; i++)
	{
		if (list[i].node.id == id1 && list[i].node.type == type)
		{
			DeleteNode(&(list[i].node), id2);
		}
	}
}

void DeleteNode(Node * n, unsigned long id)
{
	while (n->next != NULL && n->next->id != id) n = n->next;
	if (n->next == NULL)
		return;
	if (n->next->id == id)
	{
		Node* temp = n->next;
		n->next = n->next->next;
		free(temp);
	}
}
 
void InsertNode(Node * n, unsigned long id, char type)
{
	if (n == NULL)
	{
		fprintf(stderr, "insert node error\n");
		exit(0);
	}
	while (n->next != NULL) n = n->next;
	n->next = malloc(sizeof(Node));
	if (n->next == NULL)
	{
		fprintf(stderr, "malloc memory error\n");
		exit(0);
	}
	n->next->id = id;
	n->next->type = type;
	n->next->next = NULL;
}


void init_lock(SmartLock* lock) {
	pthread_mutex_init(&(lock->mutex), NULL);
	lock->id = locknumber; 
	locknumber++;
}

int lock(SmartLock* lock) {
	 
	list = AddList(list, pthread_self(), lock->id, T2L);
	if (CheckCircle(list, pthread_self(), THREADTYPE)) 
	{
		 
		DeleteList(list, pthread_self(), lock->id, T2L);
		return 0;
	}
	else
	{
		printf("%lu locking \n", pthread_self());
		 
		pthread_mutex_lock(&(lock->mutex));
		 
		DeleteList(list, pthread_self(), lock->id, T2L);
		list = AddList(list, lock->id, pthread_self(), L2T);
		return 1;
	}
}

void unlock(SmartLock* lock) {
	DeleteList(list, lock->id, pthread_self(), L2T);
	pthread_mutex_unlock(&(lock->mutex));
}

/*
 * Cleanup any dynamic allocated memory for SmartLock to avoid memory leak
 * You can assume that cleanup will always be the last function call
 * in main function of the test cases.
 */
void cleanup() {
	if (list == NULL)
		return;
	for (int i = 0; i < size; i++)
	{
		Node* temp = list[i].node.next, *temp1 = NULL;
		while (temp != NULL)
		{
			temp1 = temp->next;
			free(temp);
			temp = temp1;
		}
	}
	if (list != NULL)
		free(list);
}
