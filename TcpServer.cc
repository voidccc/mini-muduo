//author voidccc
#include <errno.h>

#include "TcpServer.h"
#include "Channel.h"
#include "Acceptor.h"
#include "TcpConnection.h"

#include <vector>
#include <iostream>

using namespace std;

TcpServer::TcpServer()
    :_epollfd(-1)
    ,_pAcceptor(NULL)
{
}

TcpServer::~TcpServer()
{
}

void TcpServer::newConnection(int sockfd)
{
    TcpConnection* tcp = new TcpConnection(_epollfd, sockfd); // Memory Leak !!!
    _connections[sockfd] = tcp;
}

void TcpServer::start()
{
    _epollfd = epoll_create(1);
    if (_epollfd <= 0)
        cout << "epoll_create error, errno:" << _epollfd << endl;
    _pAcceptor = new Acceptor(_epollfd); // Memory Leak !!!
    _pAcceptor->setCallBack(this);
    _pAcceptor->start();

    for(;;)
    {
        vector<Channel*> channels;
        int fds = epoll_wait(_epollfd, _events, MAX_EVENTS, -1);
        if(fds == -1)
        {
            cout << "epoll_wait error, errno:" << errno << endl;
            break;
        }
        for(int i = 0; i < fds; i++)
        {
            Channel* pChannel = static_cast<Channel*>(_events[i].data.ptr);
            pChannel->setRevents(_events[i].events);
            channels.push_back(pChannel);
        }

        vector<Channel*>::iterator it;
        for(it = channels.begin(); it != channels.end(); ++it)
        {
            (*it)->handleEvent();
        }
    }
}
