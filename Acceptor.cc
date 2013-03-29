//author voidccc

#include "Acceptor.h"
#include "Channel.h"
#include "IAcceptorCallBack.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>

#include <iostream>
using namespace std;

Acceptor::Acceptor(int epollfd)
    :_epollfd(epollfd)
    ,_listenfd(-1)
    ,_pAcceptChannel(NULL)
    ,_pCallBack(NULL)
{}

Acceptor::~Acceptor()
{}

void Acceptor::start()
{
    _listenfd = createAndListen();
    _pAcceptChannel = new Channel(_epollfd, _listenfd); // Memory Leak !!!
    _pAcceptChannel->setCallBack(this);
    _pAcceptChannel->enableReading();
}

int Acceptor::createAndListen()
{
    int on = 1;
    _listenfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in servaddr;
    fcntl(_listenfd, F_SETFL, O_NONBLOCK); //no-block io
    setsockopt(_listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(11111);

    if(-1 == bind(_listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)))
    {
        cout << "bind error, errno:" << errno << endl;
    }

    if(-1 == listen(_listenfd, MAX_LISTENFD))
    {
        cout << "listen error, errno:" << errno << endl;
    }
    return _listenfd;
}

void Acceptor::OnIn(int socket)
{
    int connfd;
    struct sockaddr_in cliaddr;
    socklen_t clilen = sizeof(struct sockaddr_in);
    connfd = accept(_listenfd, (sockaddr*)&cliaddr, (socklen_t*)&clilen);
    if(connfd > 0)
    {
        cout << "new connection from "
            << "[" << inet_ntoa(cliaddr.sin_addr) 
            << ":" << ntohs(cliaddr.sin_port) << "]"
            << " new socket fd:" << connfd
            << endl;
    }
    else
    {
        cout << "accept error, connfd:" << connfd
            << " errno:" << errno << endl;
    }
    fcntl(connfd, F_SETFL, O_NONBLOCK); //no-block io

    _pCallBack->newConnection(connfd);
}

void Acceptor::setCallBack(IAcceptorCallBack* pCallBack)
{
     _pCallBack = pCallBack;
}
