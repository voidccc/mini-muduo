//author voidccc
#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <sys/epoll.h>

#include "Declear.h"
#include "Define.h"
#include "IAcceptorCallBack.h"

#include <map>

using namespace std;

class TcpServer : public IAcceptorCallBack
{
    public:
        TcpServer();
        ~TcpServer();
        void start();
        virtual void newConnection(int sockfd);
    private:
        void update(Channel* pChannel, int op);

        int _epollfd;
        struct epoll_event _events[MAX_EVENTS];
        map<int, TcpConnection*> _connections;
        Acceptor* _pAcceptor;
};

#endif
