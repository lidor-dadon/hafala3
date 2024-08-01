#include <stdlib.h>
#include <sys/time.h>

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

requestQueue* initRequestQueue(int maxSize);
requestNode* initRequestNode(myRequest* request);
void push(requestQueue* queue, myRequest* request);
myRequest * popHead(requestQueue* queue);
myRequest * popFromIndex(requestQueue* queue, int index);
myRequest * popTail(requestQueue* queue);
int getSize(requestQueue* queue);
void deleteQueue(requestQueue* queue);
