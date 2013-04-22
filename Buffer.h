//author voidccc
#ifndef BUFFER_H
#define BUFFER_H

#include <string>
using namespace std;

class Buffer
{
    public:
        Buffer();
        ~Buffer();
        const char* peek();
        int readableBytes();
        void retrieve(int len);
        void append(const string& buf);
        string retrieveAllAsString();
        string retrieveAsString(size_t len);
    private:
        string _buf;
};

#endif
