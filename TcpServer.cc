//author voidccc

#include <errno.h>

#include "TcpServer.h"
#include "Channel.h"
#include "Acceptor.h"
#include "TcpConnection.h"

#include <vector>

TcpServer::TcpServer(EventLoop* pLoop)
    :_pAcceptor(NULL)
    ,_pLoop(pLoop)
    ,_pUser(NULL)
{
}

TcpServer::~TcpServer()
{
}

void TcpServer::start()
{
    _pAcceptor = new Acceptor(_pLoop); // Memory Leak !!!
    _pAcceptor->setCallback(this);
    _pAcceptor->start();
}

void TcpServer::newConnection(int sockfd)
{
    TcpConnection* tcp = new TcpConnection(sockfd, _pLoop); // Memory Leak !!!
    _connections[sockfd] = tcp;
    tcp->setUser(_pUser);
    tcp->connectEstablished();
}

void TcpServer::setCallback(IMuduoUser* user)
{
    _pUser = user;
}
