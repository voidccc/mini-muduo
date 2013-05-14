//author voidccc

#include "Buffer.h"

Buffer::Buffer()
{}

Buffer::~Buffer()
{}

const char* Buffer::peek()
{
    return _buf.c_str();
}

int Buffer::readableBytes()
{
    return static_cast<int>(_buf.size());
}

void Buffer::retrieve(int len)
{
    _buf = _buf.substr(len, _buf.size());
}

void Buffer::append(const string& data)
{
    _buf.append(data);
}

string Buffer::retrieveAllAsString()
{
    return retrieveAsString(readableBytes());
}

string Buffer::retrieveAsString(size_t len)
{
    string result(peek(), len);
    retrieve(len);
    return result;
}
