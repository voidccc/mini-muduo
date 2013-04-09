//author voidccc
#ifndef IMUDUOUSER_H
#define IMUDUOUSER_H

#include "Declear.h"
#include <string>
using namespace std;

class IMuduoUser
{
    public:
        void virtual onConnection(TcpConnection* pCon) = 0;
        void virtual onMessage(TcpConnection* pCon, string* data) = 0;
};

#endif
