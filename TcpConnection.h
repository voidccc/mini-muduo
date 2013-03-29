//author voidccc
#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include "Declear.h"
#include "IChannelCallBack.h"

class TcpConnection : public IChannelCallBack
{
    public:
        TcpConnection(int epollfd, int sockfd);
        ~TcpConnection();

        void virtual OnIn(int sockfd);
    private:
        int _epollfd;
        int _sockfd;
        Channel* _pChannel;
};
#endif
