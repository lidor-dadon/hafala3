//
// Created by AnatG on 7/31/2024.
//

#ifndef HAFALA3C_QUEUE_H
#define HAFALA3C_QUEUE_H

enum Schedalg {BLOCK = 0, DT = 1, DH = 2, BF = 3, RANDOM = 4, NOT_DEFINED};
char* schedalgArr[] = {"block", "dt", "dh", "bf", "random"};

typedef struct myRequest{
    int fd;
    double arrivalTime; //maybe struct timeval?
                        //use Time_GetSeconds() from output.c
} myRequest;

typedef struct requestNode{
    myRequest* myRequest;
    struct requestNode* prev;
    struct requestNode* next;
}requestNode;


typedef struct requestQueue{
    int size;
    requestNode * head;
    requestNode * tail;
} requestQueue;

void push(myRequest* request);
myRequest * popHead();
myRequest * popFromIndex();
myRequest * popTail();
int getSize();

#endif //HAFALA3C_QUEUE_H
