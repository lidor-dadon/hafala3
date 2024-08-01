#include "queue.h"

requestQueue* initRequestQueue(int maxSize) {
    requestQueue* newQueue = (requestQueue*)malloc(sizeof(requestQueue));
    if (newQueue != NULL) {
        newQueue->size = 0;
        newQueue->maxSize = maxSize;
        newQueue->head = NULL;
        newQueue->tail = NULL;
    }
    return newQueue;
}

requestNode* initRequestNode(myRequest* request) {
    requestNode* newNode = (requestNode*)malloc(sizeof(requestNode));
    myRequest* newRequest = (myRequest*)malloc(sizeof(myRequest));
    if (newNode != NULL) {
        newNode->next = NULL;
        newNode->prev = NULL;
        newNode->myRequest = newRequest;
        newNode->myRequest->fd = request->fd;
        newNode->myRequest->arrivalTime = request->arrivalTime;
    }
    return newNode;
}

// Function to push an element to the Queue
void push(requestQueue* queue, myRequest* request) {
    if (queue == NULL)
        return;
    if (queue->size == queue->maxSize)
        return;
    requestNode* newNode = initRequestNode(request);
    if (newNode == NULL)
        return;
    // Update the Queue's pointers
    if (queue->head == NULL) {
        queue->head = newNode;
    } else {
        requestNode* currNode = queue->head;
        while (currNode->next != NULL) {
            currNode = currNode->next;
        }
        currNode->next = newNode;
        newNode->prev = currNode;
    }
    queue->size++;
}

// Function to pop the first element from the Queue
myRequest * popHead(requestQueue* queue) {
    if (queue == NULL || queue->head == NULL)
        return NULL;
    requestNode* headQueue = queue->head;
    myRequest * request = queue->head->myRequest;
    queue->head = queue->head->next;
    if (queue->head != NULL) {
        queue->head->prev = NULL;
    }
    free(headQueue);
    queue->size--;
    return request;
}

myRequest * popFromIndex(requestQueue* queue, int index) {
    if (queue == NULL || index < 0 || index >= queue->size) {
        return NULL;
    }
    if(queue->head == NULL){
        return NULL;
    }
    requestNode* current = queue->head;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }
    if(current == queue->head) {
        queue->head = current->next;
    }
    else {
        current->prev->next = current->next;
    }
    if(current->next != NULL) {
        current->next->prev = current->prev;
    }
    myRequest *request = current->myRequest;
    free(current);
    queue->size--;
    return request;
}

myRequest * popTail(requestQueue* queue) {
    if (queue == NULL || queue->head == NULL)
        return NULL;
    requestNode* tailQueue = queue->tail;
    myRequest * request = queue->tail->myRequest;
    queue->tail = queue->tail->prev;
    if (queue->tail != NULL) {
        queue->tail->next = NULL;
    }
    free(tailQueue);
    queue->size--;
    return request;
}

int getSize(requestQueue* queue) {
    return queue->size;
}

// Function to delete the Queue
void deleteQueue(requestQueue* queue) {
    if (queue == NULL)
        return;
    if (queue->head == NULL){
        free(queue);
        return;
    }

    requestNode* current = queue->head;
    requestNode* next;

    while (current != NULL) {
        next = current->next;
        free(current->myRequest);
        free(current);
        current = next;
    }
    free(queue);
}