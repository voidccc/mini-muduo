//author voidccc
#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include "Declear.h"
#include "IChannelCallBack.h"

class TcpConnection : public IChannelCallBack
{
    public:
        TcpConnection(int sockfd, EventLoop* loop);
        ~TcpConnection();

        void virtual OnIn(int sockfd);
    private:
        int _sockfd;
        Channel* _pChannel;
        EventLoop* _loop;
};

#endif
