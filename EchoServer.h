//author voidccc
#ifndef ECHOSERVER_H
#define ECHOSERVER_H

#include "IMuduoUser.h"
#include "IRun.h"
#include "TcpServer.h"
#include "ThreadPool.h"

class EchoServer : public IMuduoUser
                   , public IRun2
{
public:
    EchoServer(EventLoop* pLoop);
    ~EchoServer();
    void start();
    virtual void onConnection(TcpConnection* pCon);
    virtual void onMessage(TcpConnection* pCon, Buffer* pBuf);
    virtual void onWriteComplate(TcpConnection* pCon);

    virtual void run2(const string& str, void* tcp);
private:
    int fib(int n);
    EventLoop* _pLoop;
    TcpServer _pServer;
    ThreadPool _threadpool;
    int _timer;
    int _index;
};

#endif
