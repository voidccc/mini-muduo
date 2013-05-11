//author voidccc

#include "EchoServer.h"
#include "TcpConnection.h"
#include "EventLoop.h"

#include <iostream>

#define MESSAGE_LENGTH 8

EchoServer::EchoServer(EventLoop* pLoop)
    :_pLoop(pLoop)
    ,_pServer(pLoop)
    ,_timer(-1)
    ,_index(0)
{
    _pServer.setCallback(this);
}

EchoServer::~EchoServer()
{}

void EchoServer::start()
{
    _pServer.start();
}

void EchoServer::onConnection(TcpConnection* pCon)
{
    cout << "onConnection" << endl;
}

void EchoServer::onMessage(TcpConnection* pCon, Buffer* pBuf)
{
    while(pBuf->readableBytes() > MESSAGE_LENGTH)
    {
        string message = pBuf->retrieveAsString(MESSAGE_LENGTH);
        pCon->send(message + "\n");
    }
    _timer = _pLoop->runEvery(0.5, this);
}
void EchoServer::onWriteComplate(TcpConnection* pCon)
{
    cout << "onWriteComplate" << endl;
}

void EchoServer::run(void* param)
{
    cout << _index << endl;
    if(_index++ == 3)
    {
        _pLoop->cancelTimer(_timer);
        _index = 0;
    }
}
