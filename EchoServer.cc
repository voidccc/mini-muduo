//author voidccc

#include "EchoServer.h"
#include "TcpConnection.h"
#include "EventLoop.h"
#include "CurrentThread.h"
#include "Task.h"

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
    _threadpool.start(3);
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
        Task task(this, message, pCon);
        _threadpool.addTask(task);
    }
}

void EchoServer::onWriteComplate(TcpConnection* pCon)
{
    cout << "onWriteComplate" << endl;
}

//run in different therad
void EchoServer::run2(const string& str, void* tcp)
{
    //IO blocking task or CPU busy task
    cout << "fib(30) = " << fib(30) << " tid = " << CurrentThread::tid() << endl;
    ((TcpConnection*)tcp)->send(str + "\n");
}

//fib is short for Fibonacci, fib is a CPU busy method
int EchoServer::fib(int n)
{
    return (n == 1 || n == 2) ? 1 : (fib(n-1) + fib(n-2));
}
