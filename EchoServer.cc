//author voidccc

#include "EchoServer.h"
#include "TcpConnection.h"

#include <iostream>

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

void EchoServer::onMessage(TcpConnection* pCon, const string& data)
{
    cout << "onmessage" << endl;
    pCon->send(data);
}
