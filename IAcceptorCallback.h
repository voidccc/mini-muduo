//author voidccc
#ifndef IACCEPTORCALLBACK_H 
#define IACCEPTORCALLBACK_H

class IAcceptorCallback
{
    public:
        virtual void newConnection(int sockfd) = 0;
};

#endif
