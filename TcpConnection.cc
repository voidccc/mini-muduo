//author voidccc

#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include "TcpConnection.h"
#include "Channel.h"
#include "EventLoop.h"
#include "Define.h"
#include "IMuduoUser.h"

#include <string.h> //for bzero
#include <iostream>
using namespace std;

TcpConnection::TcpConnection(int sockfd, EventLoop* pLoop)
    :_sockfd(sockfd)
    ,_pLoop(pLoop)
    ,_pUser(NULL)
{
    _pSocketChannel = new Channel(_pLoop, _sockfd); // Memory Leak !!!
    _pSocketChannel->setCallback(this);
    _pSocketChannel->enableReading();
}

TcpConnection::~TcpConnection()
{}

void TcpConnection::handleRead()
{
    int sockfd = _pSocketChannel->getfd();
    int readlength;
    char line[MAX_LINE];
    if(sockfd < 0)
    {
        cout << "EPOLLIN sockfd < 0 error " << endl;
        return;
    }
    bzero(line, MAX_LINE);
    if((readlength = read(sockfd, line, MAX_LINE)) < 0)
    {
        if(errno == ECONNRESET)
        {
            cout << "ECONNREST closed socket fd:" << sockfd << endl;
            close(sockfd);
        }
    }
    else if(readlength == 0)
    {
        cout << "read 0 closed socket fd:" << sockfd << endl;
        close(sockfd);
    }
    else
    {
        string linestr(line, readlength);
        _inBuf.append(linestr);
        _pUser->onMessage(this, &_inBuf);
    }
}

void TcpConnection::handleWrite()
{
    int sockfd = _pSocketChannel->getfd();
    if(_pSocketChannel->isWriting())
    {
        int n = ::write(sockfd, _outBuf.peek(), _outBuf.readableBytes());
        if( n > 0)
        {
            cout << "write " << n << " bytes data again" << endl;
            _outBuf.retrieve(n);
            if(_outBuf.readableBytes() == 0)
            {
                _pSocketChannel->disableWriting(); //remove EPOLLOUT
                _pLoop->queueLoop(this, NULL); //invoke onWriteComplate
            }
        }
    }
}

void TcpConnection::send(const string& message)
{
    int n = 0;
    if(_outBuf.readableBytes() == 0)
    {
        n = ::write(_sockfd, message.c_str(), message.size());
        if(n < 0)
            cout << "write error" << endl;
        if(n == static_cast<int>(message.size()))
            _pLoop->queueLoop(this, NULL); //invoke onWriteComplate
    }

    if( n < static_cast<int>(message.size()))
    {
        _outBuf.append(message.substr(n, message.size()));
        if(_pSocketChannel->isWriting())
        {
            _pSocketChannel->enableWriting(); //add EPOLLOUT
        }
    }
}

void TcpConnection::connectEstablished()
{
    if(_pUser)
    _pUser->onConnection(this);
}

void TcpConnection::setUser(IMuduoUser* user)
{
    _pUser = user;
}

void TcpConnection::run(void* param)
{
    _pUser->onWriteComplate(this);
}
