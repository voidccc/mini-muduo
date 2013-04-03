//author voidccc
#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include "Declear.h"
#include "IChannelCallback.h"

#include <string>
using namespace std;

class TcpConnection : public IChannelCallback
{
    public:
        TcpConnection(int sockfd, EventLoop* pLoop);
        ~TcpConnection();
        void send(const string& message);
        void connectEstablished();
        void setUser(IMuduoUser* pUser);

        void setCallback(IAcceptorCallback* pCallback);
        void virtual onIn(int sockfd);
    private:
        int _sockfd;
        Channel* _pChannel;
        EventLoop* _pLoop;
        IMuduoUser* _pUser;
};

#endif
