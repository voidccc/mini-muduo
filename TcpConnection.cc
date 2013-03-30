//author voidccc

#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include "TcpConnection.h"
#include "Channel.h"
#include "EventLoop.h"
#include "Define.h"

#include <string.h> //for bzero
#include <iostream>
using namespace std;

TcpConnection::TcpConnection(int sockfd, EventLoop* loop)
    :_sockfd(sockfd)
    ,_loop(loop)
{
    _pChannel = new Channel(_loop, _sockfd); // Memory Leak !!!
    _pChannel->setCallBack(this);
    _pChannel->enableReading();
}

TcpConnection::~TcpConnection()
{}

void TcpConnection::OnIn(int sockfd)
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
        if(write(sockfd, line, readlength) != readlength)
            cout << "error: not finished one time" << endl;
    }
}
