//author voidccc
#ifndef EPOLL_H
#define EPOLL_H

#include <sys/epoll.h>

#include "Declear.h"
#include "Define.h"

#include <vector>
using namespace std;

class Epoll
{
public:
    Epoll();
    ~Epoll();
    void poll(vector<Channel*>* pChannels);
    void update(Channel* channel);
private:
    int _epollfd;
    struct epoll_event _events[MAX_EVENTS];
};

#endif
