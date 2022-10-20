/* Term Project
 * Aster Li
 * 6/4/2022
 * Linked list of arrays where each array is a circular pqueue.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define arrayLength 10

//Declaring data structure
typedef struct list{
    int count;
    struct node *head;
    struct node *tail;
} LIST;
typedef struct node{
    struct pqueue *pq; //pointer to pqueue struct
    struct node *next;
    struct node *prev;
} NODE;
typedef struct pqueue {
    int first; //first variable for circular priority queue
    int count;
    int length;
    void **data; //priority queue array of void data type
} PQ;

//Private functions

//Creates new priority queue array
//Big-O: O(1)
PQ *createQueue(void){
    PQ *pq;
    pq = malloc(sizeof(PQ));
    assert (pq != NULL);
    //initialize first, count, and length
    pq->first = 0; //index of first element of priority queue
    pq->count = 0;
    pq->length = arrayLength;
    //initializing array of void data
    pq->data = malloc(sizeof(void*) * pq->length);
    assert(pq->data != NULL);
    return pq;
}

//Add an entry to the circular priority queue array pointed by pq
//Big-O: O(1)
void addEntry(PQ *pq, void *entry, int flag){
    assert (pq != NULL && entry != NULL);
    assert(pq->count < pq->length); //must be space in array
    if (flag == 0){
        //add last element
        pq->data[(pq->count + pq->first) % pq->length] = entry;
    } else if (flag == 1){
        //add first element
        pq->data[(pq->first - 1 + pq->length) % pq->length] = entry;
        pq->first = (pq->first - 1 + pq->length) % pq->length;
    }
    pq->count++;
    return;
}

//Remove an entry from the circular priority queue pointed by pqeue
//Big-O: O(1)
void *removeEntry(PQ *pq, int flag){
    assert (pq != NULL);
    assert (pq->count > 0); //there must be an element in the array
    void *remove;
    if (flag == 0){
        //remove first element
        remove = pq->data[pq->first];
        pq->first = (pq->first + 1) % pq->length;
    } else if (flag == 1) {
        //remove last element
        remove = pq->data[(pq->first + pq->count - 1) % pq->length];
    }
    pq->count--;
    return(remove);
}

//Interface functions

//Creates linked list with a pqueue in each node
//Big-O: O(1)
LIST *createList(void){
    LIST *lp = malloc(sizeof(LIST));
    assert (lp != NULL);
    //initialize count, head, and tail
    lp->count = 1;
    lp->head = malloc(sizeof(NODE));
    assert (lp->head != NULL);
    //head and tail point to the same node when there is only 1
    lp->tail = lp->head;
    //set doubly linked list
    lp->head->next = NULL;
    lp->head->prev = NULL;
    lp->head->pq = createQueue(); //create a pqueue in lp->head
    return lp;
}

//Frees linked list and all the arrays within each node
//Big-O: O(n)
void destroyList(LIST *lp){
    assert(lp != NULL);
    NODE *pDel, *pNext;
    pDel = lp->head;
    //traverse linked list leaving behind a pointer to free each node
    while(pNext != NULL) {
        pNext = pDel->next;
        //free pointer to array, free pq pointer, free node itself
        free(pDel->pq->data);
        free(pDel->pq);
        free(pDel);
        pDel = pNext;
    }
    free(lp);
    return;
}

//Returns number of elements in all arrays in linked list
//Big-O: O(n)
int numItems(LIST *lp){
    assert(lp != NULL);
    NODE *pCur = lp->head;
    int sum = 0;
    //traverses list and sums number of elemnts in each array in node
    while (pCur != NULL){
        sum += pCur->pq->count;
        pCur = pCur->next;
    }
    return sum;
}

//Adds item to the beginning of a pqueue in the first node
//If first node is full, appends new node to beginning of linked list
//Big-O: O(1)
void addFirst(LIST *lp, void *item){
    assert(lp != NULL && item != NULL);
    if (lp->head->pq->count == arrayLength){
        NODE *pNew;
        pNew = malloc(sizeof(NODE)); //allocates new node to be added
        assert (pNew != NULL);
        pNew->pq = createQueue();
        addEntry(pNew->pq, item, 1);
        pNew->next = lp->head;
        pNew->prev = NULL;
        lp->head->prev = pNew;
        lp->head = pNew;
        lp->count++;
    } else {
        addEntry(lp->head->pq, item, 1);
    }
    return;
}

//Adds item to the end of a pqueue in the last node
//If last node is full, appends new node to end of linked list
//Big-O: O(1)
void addLast(LIST *lp, void *item){
    assert(lp != NULL && item != NULL);
    if (lp->head->pq->count == arrayLength){
        NODE *pNew;
        pNew = malloc(sizeof(NODE)); //allocates new node to be added
        assert (pNew != NULL);
        pNew->pq = createQueue();
        addEntry(pNew->pq, item, 0);
        pNew->prev = lp->tail;
        pNew->next = NULL;
        lp->tail->next = pNew;
        lp->tail = pNew;
        lp->count++;
    } else {
        addEntry(lp->tail->pq, item, 0);
    }
    return;
}

//Removes first item from the beginning of a pqueue in the first node
//If first node is empty, deletes head and sets head to next node of linked list
//Big-O: O(n)
void *removeFirst(LIST *lp){
    assert(lp != NULL);
    NODE *pDel = lp->head;
    while (pDel->pq->data == NULL || pDel->pq->count == 0){
        lp->head = pDel->next;
        free(pDel->pq->data);
        free(pDel->pq);
        free(pDel);
        lp->count--;
        pDel = lp->head;
    }
    return (removeEntry(lp->head->pq, 0));
}

//Removes last item from the end of a pqueue in the last node
//If last node is empty, deletes tail and sets tail to previous node of linked list
//Big-O: O(n)
void *removeLast(LIST *lp){
    assert(lp != NULL);
    NODE *pDel = lp->tail;
    while (pDel->pq->data == NULL || pDel->pq->count == 0){
        lp->tail = pDel->prev;
        free(pDel->pq->data);
        free(pDel->pq);
        free(pDel);
        lp->count--;
        pDel = lp->tail;
    }
    return (removeEntry(lp->tail->pq, 1));
}

//Returns first item in first node of linked list
//Big-O: O(1)
void *getFirst(LIST *lp){
    assert (lp != NULL);
    return(lp->head->pq->data[lp->head->pq->first]);
}

//Returns last item in last node of linked list
//Big-O: O(1)
void *getLast(LIST *lp){
    assert (lp != NULL);
    return(lp->tail->pq->data[(lp->tail->pq->first + lp->tail->pq->count -1) % lp->tail->pq->length]);
}

//Traverses each array in linked list and returns the item in the given index
//Big-O: O(n)
void *getItem(LIST *lp, int index){
    int ind = index;
    assert(lp != NULL && index >= 0);
    NODE *pCur = lp->head;
    while (pCur != NULL){
        if (ind - pCur->pq->count < 0){//the item we want is within the size of the current array
            return (pCur->pq->data[(pCur->pq->first + ind) % pCur->pq->length]);
        } else {//the index is larger than the array count, keep traversing and update index
            ind = ind - pCur->pq->count;
            pCur = pCur->next;
        }
    }
    return NULL;
}

//Traverses each array in linked list and puts a new item in the given index
//Big-O: O(n)
void setItem(LIST *lp, int index, void *item){
    int ind = index;
    assert(lp != NULL && item != NULL && index >= 0);
    NODE *pCur = lp->head;
    while (pCur != NULL){
        if (ind - pCur->pq->count < 0){//the index we want is within the size of the current array
            pCur->pq->data[(pCur->pq->first + ind) % pCur->pq->length] = item;
            return;
        } else {//the index is larger than the array count, keep traversing and update index
            ind = ind - pCur->pq->count;
            pCur = pCur->next;
        }
    }
}