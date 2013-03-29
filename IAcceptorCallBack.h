//author voidccc
#ifndef IACCEPTORCALLBACK_H 
#define IACCEPTORCALLBACK_H

class IAcceptorCallBack
{
    public:
        void virtual newConnection(int sockfd){};
};

#endif
