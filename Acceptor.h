//author voidccc
#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include "Declear.h"
#include "Define.h"
#include "IChannelCallback.h"

class Acceptor : public IChannelCallback
{
    public:
        Acceptor(EventLoop* pLoop);
        ~Acceptor();

        void start();
        void setCallback(IAcceptorCallback* pCallback);
        virtual void handleRead();
        virtual void handleWrite();
    private:
        int createAndListen();
        int _listenfd;
        Channel* _pSocketAChannel;
        IAcceptorCallback* _pCallback;
        EventLoop* _pLoop;
};

#endif
