//author voidccc
#ifndef ICHANNELCALLBACK_H
#define ICHANNELCALLBACK_H

class IChannelCallback
{
    public:
        void virtual handleRead() = 0;
        void virtual handleWrite() = 0;
};

#endif
