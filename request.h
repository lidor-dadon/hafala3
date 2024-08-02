#ifndef __REQUEST_H__
#include "requestMenager.h"

typedef struct Threads_stats{
    int id;
    int stat_req;
    int dynm_req;
    int total_req;
} * threads_stats;

//void requestHandle(int fd, struct timeval arrival, struct timeval dispatch, threads_stats t_stats);
void requestHandle(int fd, struct timeval arrival, struct timeval dispatch, threads_stats t_stats, requestManager* manager);

#endif
