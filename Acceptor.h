//author voidccc
#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include "Declear.h"
#include "Define.h"
#include "IChannelCallBack.h"

class Acceptor : public IChannelCallBack
{
    public:
        Acceptor(int epollfd);
        ~Acceptor();

        void virtual OnIn(int socket);
        void setCallBack(IAcceptorCallBack* pCallBack);
        void start();
    private:
        int createAndListen();
        int _epollfd;
        int _listenfd;
        Channel* _pAcceptChannel;
        IAcceptorCallBack* _pCallBack;
};
#endif
