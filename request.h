#ifndef __REQUEST_H__

void requestHandle(int fd, struct timeval arrival, struct timeval dispatch, threads_stats t_stats);

#endif
