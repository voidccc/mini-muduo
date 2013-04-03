//author voidccc
#ifndef ICHANNELCALLBACK_H
#define ICHANNELCALLBACK_H

class IChannelCallback
{
    public:
        void virtual onIn(int sockfd) = 0;
};

#endif
