#ifndef HAFALA3C_QUEUE_H
#define HAFALA3C_QUEUE_H
#include <stdlib.h>
#include <sys/time.h>


enum Schedalg {BLOCK = 0, DT = 1, DH = 2, BF = 3, RANDOM = 4, NOT_DEFINED};
char* schedalgArr[] = {"block", "dt", "dh", "bf", "random"};

typedef struct myRequest{
    int fd;
    struct timeval arrivalTime;
                        //use Time_GetSeconds() from output.c
} myRequest;

typedef struct requestNode{
    myRequest* myRequest;
    struct requestNode* prev;
    struct requestNode* next;
} requestNode;


typedef struct requestQueue{
    int size;
    int maxSize;
    requestNode * head;
    requestNode * tail;
} requestQueue;

requestNode* initRequestNode(myRequest* request);
void push(requestQueue* queue, myRequest* request);
myRequest * popHead(requestQueue* queue);
myRequest * popFromIndex(requestQueue* queue, int index);
myRequest * popTail(requestQueue* queue);
int getSize(requestQueue* queue);
void deleteQueue(requestQueue* queue);

#endif //HAFALA3C_QUEUE_H
