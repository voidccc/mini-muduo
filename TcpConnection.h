//author voidccc
#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include "Declear.h"
#include "IChannelCallback.h"
#include "Buffer.h"
#include "IRun.h"

#include <string>
using namespace std;

class TcpConnection : public IChannelCallback
                      , public IRun
{
    public:
        TcpConnection(int sockfd, EventLoop* pLoop);
        ~TcpConnection();
        void send(const string& message);
        void connectEstablished();
        void setUser(IMuduoUser* pUser);

        void setCallback(IAcceptorCallback* pCallback);
        void virtual handleRead();
        void virtual handleWrite();
        void virtual run(void* param);
    private:
        int _sockfd;
        Channel* _pSocketChannel;
        EventLoop* _pLoop;
        IMuduoUser* _pUser;
        Buffer _inBuf;
        Buffer _outBuf;
};

#endif
