//author voidccc

#include "TcpServer.h"
#include "EventLoop.h"
#include "EchoServer.h"

int main(int args, char** argv)
{
    EventLoop loop;
    EchoServer echoserver(&loop);
    echoserver.start();
    loop.loop();
    return 0;
}
