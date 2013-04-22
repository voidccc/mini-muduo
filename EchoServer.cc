//author voidccc

#include "EchoServer.h"
#include "TcpConnection.h"

#include <iostream>

#define MESSAGE_LENGTH 8

EchoServer::EchoServer(EventLoop* pLoop)
    :_pLoop(pLoop)
    ,_pServer(pLoop)
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
}
void EchoServer::onWriteComplate(TcpConnection* pCon)
{
    cout << "onWriteComplate" << endl;
}
