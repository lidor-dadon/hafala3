#include "queue.h"

requestNode* initRequestNode(myRequest* request) {
    requestNode* newNode = (requestNode*)malloc(sizeof(requestNode));
    if (newNode != NULL) {
        newNode->next = NULL;
        newNode->prev = NULL;
        newNode->myRequest->fd = request->fd;
        newNode->myRequest->arrivalTime = request->arrivalTime;
    }
    return newNode;
}

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

myRequest * popHead(requestQueue* queue) {
    if (queue == NULL || queue->head == NULL)
        return NULL;
    requestNode* headQueue = queue->head;
    queue->head = queue->head->next;
    if (queue->head != NULL) {
        queue->head->prev = NULL;
    }
    queue->size--;
    headQueue->next = NULL;
    headQueue->prev = NULL;
    return headQueue->myRequest;
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
    current->next = NULL;
    current->prev = NULL;
    queue->size--;
    return current->myRequest;
}

myRequest * popTail(requestQueue* queue) {
    if (queue == NULL || queue->head == NULL)
        return NULL;
    requestNode* tailQueue = queue->tail;
    queue->tail = queue->tail->prev;
    if (queue->tail != NULL) {
        queue->tail->next = NULL;
    }
    queue->size--;
    tailQueue->next = NULL;
    tailQueue->prev = NULL;
    return tailQueue->myRequest;
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
        free(current);
        current = next;
    }
    free(queue);
    return;
}
