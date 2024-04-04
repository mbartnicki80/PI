#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

typedef struct {
    double *results;
    int len;
} Data;

typedef struct tagQueueElement {
    Data data;
    struct tagQueueElement *next;
} QueueElement;

typedef struct tagQueue {
    int sum;
    QueueElement *head;
    QueueElement *tail;
} Queue;

void free_queue(Queue *pqueue) {
    QueueElement *curr = pqueue->head;
    QueueElement *next = NULL;
    while (curr!=NULL) {
        next = curr->next;
        free(curr);
        curr = next;
    }
}

void push_copy(Queue *pqueue, const Data *pdata) {
    QueueElement *new = (QueueElement *)malloc(sizeof(QueueElement));
    new->data = *pdata;
    new->next = NULL;
    if (pqueue->head!=NULL) {
        pqueue->tail->next = new;
        pqueue->tail = pqueue->tail->next;
    }
    else
        pqueue->head = pqueue->tail = new;
    pqueue->sum += pdata->len;
}

int peek(const Queue *pqueue, Data *pdata) {
    if (pqueue->head==NULL)
        return 0;
    *pdata = pqueue->head->data;
    return 1;
}

int pop(Queue *pqueue, Data *pdata) {
    if (pqueue->head==NULL)
        return -1;
    *pdata = pqueue->head->data;
    QueueElement *del = pqueue->head;
    pqueue->head = pqueue->head->next;
    pqueue->sum -= del->data.len;
    free(del);
    if (pqueue->head==NULL) {
        pqueue->tail = NULL;
        return 0;
    }
    return 1;
}

int get_total_count(const Queue *pqueue) {
    return pqueue->sum;
}

int main(void) {

}