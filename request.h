#ifndef __REQUEST_H__

void requestHandle(int fd, pThread currentThread, struct timeval arriveTime, struct timeval dispatchTime);

#endif
