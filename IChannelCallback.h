//author voidccc
#ifndef ICHANNELCALLBACK_H
#define ICHANNELCALLBACK_H

class IChannelCallback
{
    public:
        virtual void handleRead() = 0;
        virtual void handleWrite() = 0;
};

#endif
