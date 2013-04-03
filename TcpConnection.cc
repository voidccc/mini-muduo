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
    _pChannel = new Channel(_pLoop, _sockfd); // Memory Leak !!!
    _pChannel->setCallback(this);
    _pChannel->enableReading();
}

TcpConnection::~TcpConnection()
{}

void TcpConnection::onIn(int sockfd)
{
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
        string buf(line, MAX_LINE);
        _pUser->onMessage(this, buf);
    }
}

void TcpConnection::send(const string& message)
{
    int n = ::write(_sockfd, message.c_str(), message.size());
    if( n != static_cast<int>(message.size()))
        cout << "write error ! " << message.size() - n << "bytes left" << endl;
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
