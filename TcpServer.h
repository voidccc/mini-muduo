//author voidccc
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h> //for bzero
#include <iostream>

#define MAX_LINE 100
#define MAX_EVENTS 500
#define MAX_LISTENFD 5

using namespace std;

class TcpServer
{
    public:
        TcpServer();
        ~TcpServer();
        void start();
    private:
        int createAndListen();
};
