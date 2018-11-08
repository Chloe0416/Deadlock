#ifndef __KLOCK_H__
#define __KLOCK_H__

#include <pthread.h>
#include <stdlib.h>
typedef struct {
	 pthread_mutex_t mutex;
	 unsigned long id;
} SmartLock;

void init_lock(SmartLock* lock);
int lock(SmartLock* lock);
void unlock(SmartLock* lock);
void cleanup();

#define THREADTYPE  0
#define LOCKTYPE 1
#define T2L 0
#define L2T 1
 
typedef struct node {
	struct node* next;
	unsigned long int id; 
	char type; 
} Node;

typedef struct
{
	Node node;
	char check; 
} List;


int size;
 
List * AddList(List *list, unsigned long id1, unsigned long id2, char type);

int CheckCircle(List * list, unsigned long id, char type);
 
void DeleteLIst(List *list, unsigned long id1, unsigned long id2, char type);
 
void DeleteNode(Node* n, unsigned long id);
 
void InsertNode(Node* n, unsigned long id, char type);

#endif
