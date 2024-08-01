#include "queue.h"
#include <pthread.h>

typedef pthread_cond_t cond;       // Typedef for condition variable
typedef pthread_mutex_t mutex;     // Typedef for mutex

typedef struct requestManager {
    mutex mutexLock;
    cond waitAndRunListEmptySignal; // Condition variable for signaling when the waitList is empty
    cond waitListNotEmptySignal;    // Condition variable for signaling when the waitList is not empty
    cond runListNotFullSignal;      // Condition variable for signaling when the runList is not full
    requestQueue* waitQueue;        // Pointer to the queue of pending requests
    int runQueueSize;                // running requests amount
    int maxRequests;                // Maximum number of requests allowed
} requestManager;

requestManager* creatManager(int maxRequests);
void destroyManager(requestManager* requestManager);