//author voidccc
#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <sys/epoll.h>

#include "Declear.h"
#include "Define.h"
#include "IAcceptorCallback.h"
#include "IMuduoUser.h"

#include <map>
using namespace std;

class TcpServer : public IAcceptorCallback
{
    public:
        TcpServer(EventLoop* pLoop);
        ~TcpServer();
        void start();
        void setCallback(IMuduoUser* pUser);
        virtual void newConnection(int sockfd);
    private:
        struct epoll_event _events[MAX_EVENTS];
        map<int, TcpConnection*> _connections;
        Acceptor* _pAcceptor;
        EventLoop* _pLoop;
        IMuduoUser* _pUser;
};

#endif
