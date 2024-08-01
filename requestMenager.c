#include "requestMenager.h"


requestManager* creatManager(int maxRequests) {
    requestQueue* waitQueue = initRequestQueue(maxRequests);
    if (waitQueue == NULL) {
        return NULL;
    }

    requestManager* newRequestManager = (requestManager*)malloc(sizeof(requestManager));
    if (newRequestManager == NULL) {
        free(waitQueue);
        return NULL;
    }

    // Initialize the mutex associated with the manager
    if (pthread_mutex_init(&(newRequestManager->mutexLock), NULL) != 0) {
        // Handle mutex initialization failure
        free(waitQueue);       // Free allocated memory
        free(newRequestManager); // Free allocated memory
        return NULL;
    }

    // Initialize the condition variables associated with the manager
    if (pthread_cond_init(&newRequestManager->waitAndRunListEmptySignal, NULL) != 0 ||
        pthread_cond_init(&newRequestManager->waitListNotEmptySignal, NULL) != 0 ||
        pthread_cond_init(&newRequestManager->runListNotFullSignal, NULL) != 0)
    {
        pthread_mutex_destroy(&newRequestManager->mutexLock);
        free(waitQueue);
        free(newRequestManager);
        return NULL;
    }

    // Initialize other members of the RequestManager structure
    newRequestManager->maxRequests = maxRequests;
    newRequestManager->waitQueue = waitQueue;
    newRequestManager->runQueueSize = 0;

    return newRequestManager;
}






void destroyManager(requestManager* requestManager) {
    if (requestManager == NULL) {
        return;
    }
    pthread_mutex_destroy(&requestManager->mutexLock);

    pthread_cond_destroy(&requestManager->waitAndRunListEmptySignal);
    pthread_cond_destroy(&requestManager->waitListNotEmptySignal);
    pthread_cond_destroy(&requestManager->runListNotFullSignal);

    deleteQueue(requestManager->waitQueue);

    free(requestManager);
}

int main () {
    return 0;
}



