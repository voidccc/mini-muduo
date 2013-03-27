//author voidccc
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>

#include "TcpServer.h"
#include "Channel.h"

#include <string.h> //for bzero
#include <iostream>
#include <vector>

using namespace std;

TcpServer::TcpServer()
    :_epollfd(-1)
    ,_listenfd(-1)
{
}

TcpServer::~TcpServer()
{
}

int TcpServer::createAndListen()
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

void TcpServer::OnIn(int sockfd)
{
    cout << "OnIn:" << sockfd<< endl;
    if(sockfd == _listenfd)
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

        // Memory Leak !!!
        Channel* pChannel = new Channel(_epollfd, connfd);
        pChannel->setCallBack(this);
        pChannel->enableReading();
    }
    else
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
}

void TcpServer::start()
{
    _epollfd = epoll_create(1);
    if (_epollfd <= 0)
        cout << "epoll_create error, errno:" << _epollfd << endl;
    _listenfd = createAndListen();

    Channel* pChannel = new Channel(_epollfd, _listenfd);
    pChannel->setCallBack(this);
    pChannel->enableReading();

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
