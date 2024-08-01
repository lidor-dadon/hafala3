#include "segel.h"
#include "request.h"

#include "queue.h"

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
        fprintf(stderr, "<schedalg> must be\n", argv[0]); //TODO:find what to print
        exit(1);
    }
}

void * tread_main(void*){

}

int main(int argc, char *argv[])
{
    int listenfd, connfd, port, clientlen;
    struct sockaddr_in clientaddr;

    int threads, queueSize;
    enum Schedalg* schedalg;

    getargs(&port, &threads, &queueSize, schedalg, argc, argv);

    // 
    // HW3: Create some threads...
    //
    //allocate array of int/struct for passing information to the treads
    pthread_t threadsArr[threads];
    for ( int i=0; i<threads; i++) {
        pthread_create(&threadsArr[i], NULL, tread_main, NULL);
    }

    myRequest* request = (myRequest*) malloc(sizeof (myRequest));
    if(request == NULL){
        fprintf(stderr, "Allocation error\n", argv[0]); //TODO:find what to print
        exit(1);
    }
    listenfd = Open_listenfd(port);
    pthread_mutex_t m;
    pthread_mutex_init(&m,NULL);
    pthread_cond_t queueNotEmpty;
    pthread_cond_t queueNotFull;
    pthread_cond_init(&queueNotEmpty,NULL);
    pthread_cond_init(&queueNotFull,NULL);


    while (1) {
	clientlen = sizeof(clientaddr);
	connfd = Accept(listenfd, (SA *)&clientaddr, (socklen_t *) &clientlen);
    request->fd = connfd;
    gettimeofday(&request->arrivalTime, NULL);
    //push(waitQueue,request);


    //
	// HW3: In general, don't handle the request in the main thread.
	// Save the relevant info in a buffer and have one of the worker threads 
	// do the work. 
	// 
	requestHandle(connfd);

	Close(connfd);
    }
}

    


 
