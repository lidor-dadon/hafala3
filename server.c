#include "segel.h"
#include "request.h"

#define SCHEDALG_LENGTH 7

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

void getargs(int *port, int* threads, int* queueSize, char schedalg[], int argc, char *argv[])
{
    if (argc < 5) {
        fprintf(stderr, "Usage: %s <port> <threads> <queue_size> <schedalg>\n", argv[0]);
        exit(1);
    }
    *port = atoi(argv[1]);
    *threads = atoi(argv[2]);
    *queueSize = atoi(argv[3]);
    int i = 0;
    for(; argv[4][i] && i<SCHEDALG_LENGTH -1; i++){
        schedalg[i] = argv[4][i];
    }
    schedalg[i] = '\0';
    //if(strcmp(schedalg, ""))
}


int main(int argc, char *argv[])
{
    int listenfd, connfd, port, clientlen;
    struct sockaddr_in clientaddr;

    int threads, queueSize;
    char schedalg[SCHEDALG_LENGTH];

    getargs(&port, &threads, &queueSize, schedalg, argc, argv);

    // 
    // HW3: Create some threads...
    //

    listenfd = Open_listenfd(port);
    while (1) {
	clientlen = sizeof(clientaddr);
	connfd = Accept(listenfd, (SA *)&clientaddr, (socklen_t *) &clientlen);

	// 
	// HW3: In general, don't handle the request in the main thread.
	// Save the relevant info in a buffer and have one of the worker threads 
	// do the work. 
	// 
	requestHandle(connfd);

	Close(connfd);
    }

}


    


 
