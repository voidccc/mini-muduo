//author voidccc
#ifndef ECHOSERVER_H
#define ECHOSERVER_H

#include "IMuduoUser.h"
#include "IRun.h"
#include "TcpServer.h"

class EchoServer : public IMuduoUser
                   , public IRun
{
public:
    EchoServer(EventLoop* pLoop);
    ~EchoServer();
    void start();
    void virtual onConnection(TcpConnection* pCon);
    void virtual onMessage(TcpConnection* pCon, Buffer* pBuf);
    void virtual onWriteComplate(TcpConnection* pCon);

    void virtual run(void* param);
private:
    EventLoop* _pLoop;
    TcpServer _pServer;
    int _timer;
    int _index;
};

#endif
