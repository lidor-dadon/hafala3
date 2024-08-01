#include "segel.h"
#include "request.h"

#include "queue.h"
#include "requestMenager.h"

#define SCHEDALG_LENGTH 5
enum Schedalg {BLOCK = 0, DT = 1, DH = 2, BF = 3, RANDOM = 4, NOT_DEFINED};
char* schedalgArr[] = {"block", "dt", "dh", "bf", "random"};

// 
// server.c: A very, very simple web server
//
// To run:
//  ./server <portnum (above 2000)>
//
// Repeatedly handles HTTP requests sent to this port number.
// Most of the work is done within routines written in request.c
//

// HW3: Parse the new arguments too
/*void getargs(int *port, int argc, char *argv[])
{
    if (argc < 2) {
	fprintf(stderr, "Usage: %s <port>\n", argv[0]);
	exit(1);
    }
    *port = atoi(argv[1]);
}*/



void getargs(int *port, int* threads, int* queueSize, enum Schedalg* schedalg, int argc, char *argv[])
{
    if (argc < 5) {
        fprintf(stderr, "Usage: %s <port> <threads> <queue_size> <schedalg>\n", argv[0]);
        exit(1);
    }
    *port = atoi(argv[1]);
    *threads = atoi(argv[2]);
    *queueSize = atoi(argv[3]);
    *schedalg = NOT_DEFINED;
    for(int i = 0; i<SCHEDALG_LENGTH; i++){
        if(strcmp(argv[4], schedalgArr[i])){
            *schedalg = i;
        }
    }
    if (*schedalg == NOT_DEFINED){
        fprintf(stderr, "<schedalg> must be\n"); //TODO:find what to print
        exit(1);
    }
}

typedef struct passToThead{
    requestManager* manager;
    int threadId;
} passToThead;

void * tread_main(void* parameters){
    passToThead * params = (passToThead*)parameters;
    requestManager* manager = params->manager;
    int threadId = params->threadId;
    myRequest * request;
    while(1){
        pthread_mutex_lock(&manager->mutexLock);
        while(manager->waitQueue->size == 0){
            pthread_cond_wait(&manager->waitListNotEmptySignal, &manager->mutexLock);
        }
        request = popHead(manager->waitQueue);
        manager->runQueueSize++;
        pthread_cond_signal(&manager->runListNotFullSignal);
        pthread_mutex_unlock(&manager->mutexLock);

        requestHandle(request->fd);

        pthread_mutex_lock(&manager->mutexLock);
        manager->runQueueSize--;
        if(manager->waitQueue->size == 0 && manager->runQueueSize == 0){
            pthread_cond_signal(&manager->waitAndRunListEmptySignal);
        }
        pthread_mutex_unlock(&manager->mutexLock);

        Close(request->fd);
        free(request);
        //sleep(20);

    }

}

int main(int argc, char *argv[])
{
    int listenfd, connfd, port, clientlen;
    struct sockaddr_in clientaddr;

    int threads, queueSize;
    enum Schedalg schedalg;

    getargs(&port, &threads, &queueSize, &schedalg, argc, argv);
    myRequest* request = (myRequest*) malloc(sizeof (myRequest));
    if(request == NULL){
        fprintf(stderr, "Allocation error\n"); //TODO:find what to print
        exit(1);
    }
    requestManager* manager = creatManager(queueSize);

    // 
    // HW3: Create some threads...
    //
    //allocate array of int/struct for passing information to the treads
    passToThead* passToTheadArr = (passToThead*)malloc(threads* sizeof(passToThead));
    pthread_t threadsArr[threads];
    for ( int i=0; i<threads; i++) {
        passToTheadArr[i].manager = manager;
        passToTheadArr[i].threadId = i;
        pthread_create(&threadsArr[i], NULL, tread_main, (void*)(&passToTheadArr[i]));
    }

    listenfd = Open_listenfd(port);
    while (1) {
        clientlen = sizeof(clientaddr);
        connfd = Accept(listenfd, (SA *)&clientaddr, (socklen_t *) &clientlen);
        request->fd = connfd;
        gettimeofday(&request->arrivalTime, NULL);

        //push request to queue
        pthread_mutex_lock(&manager->mutexLock);
        if(manager->waitQueue->size + manager->runQueueSize >= manager->maxRequests){
            if(schedalg == BLOCK){
                while(manager->waitQueue->size + manager->runQueueSize >= manager->maxRequests){
                    pthread_cond_wait(&manager->runListNotFullSignal, &manager->mutexLock);
                }
            }
            if(schedalg == DT){
                pthread_cond_signal(&manager->waitListNotEmptySignal);
                Close(connfd);
                pthread_mutex_unlock(&manager->mutexLock);
                continue;
            }
            if(schedalg == DH){
                if(manager->waitQueue->size != 0 ) {
                    myRequest* request = popHead(manager->waitQueue);
                    Close(request->fd);
                    free(request);
                }
                else{
                    close(connfd);
                    pthread_mutex_unlock(&manager->mutexLock);
                    continue;
                }
            }
            if(schedalg == BF){
                while((manager->waitQueue->size + manager->runQueueSize) > 0){
                    pthread_cond_wait(&manager->waitAndRunListEmptySignal, &manager->mutexLock);
                }
                close(connfd);
                pthread_mutex_unlock(&manager->mutexLock);
                continue;
            }
            if(schedalg == RANDOM){
                int index;
                int drop = manager->waitQueue->size/2 + manager->waitQueue->size%2;
                myRequest* request;
                if(drop != 0){
                    for(int i = 0 ; i < drop ; i++){
                        index = rand()%(manager->waitQueue->size);
                        request = popFromIndex(manager->waitQueue, index);
                        Close(request->fd);
                        free(request);
                    }
                }
                else{
                    close(connfd);
                    pthread_mutex_unlock(&manager->mutexLock);
                    continue;
                }
            }
        }
        push(manager->waitQueue,request);
        pthread_cond_signal(&manager->waitListNotEmptySignal);
        pthread_mutex_unlock(&manager->mutexLock);

        //
        // HW3: In general, don't handle the request in the main thread.
        // Save the relevant info in a buffer and have one of the worker threads
        // do the work.
        //

        //requestHandle(connfd);

        Close(connfd);
    }
    destroyManager(manager);
}

    


 
