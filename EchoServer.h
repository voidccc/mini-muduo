//author voidccc
#ifndef ECHOSERVER_H
#define ECHOSERVER_H

#include "IMuduoUser.h"
#include "TcpServer.h"

class EchoServer : public IMuduoUser
{
public:
    EchoServer(EventLoop* pLoop);
    ~EchoServer();
    void start();
    void virtual onConnection(TcpConnection* pCon);
    void virtual onMessage(TcpConnection* pCon, Buffer* pBuf);
    void virtual onWriteComplate(TcpConnection* pCon);
private:
    EventLoop* _pLoop;
    TcpServer _pServer;
};

#endif
