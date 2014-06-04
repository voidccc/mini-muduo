mini-muduo
==========
a mini implementation of muduo

tags:

v0.01
1 epoll example

v0.02
1 Add TcpServer, TcpServer is the first object in code.

v0.03
1 Add Channel, Channel is the observer of socket fd.
2 Add IChannelCallback, move event handler from epoll loop to a callback function in TcpServer.

v0.04
1 Add Acceptor, Acceptor is the observer of listening socket fd.
2 Add TcpConnection, TcpConnection is the observer of read/write socket fd.

v0.05
1 Add EventLoop, EventLoop is the wrapper of "for" loop.
2 Add Epoll, Epoll is the wrapper of epoll file descriptor.

v0.06
1 Add EchoServer, EchoServer is the user of mini-muduo library.

v0.07
1 Add input/output buffer.

v0.08
1 Add input/output buffer.(Buffer and onWriteComplate)

v0.09
1 Add Timer.

v0.10
1 Better naming convention

v0.11
1 remove muti-thread specific code
2 32-bit/64-bit support

v0.12
1 Foundations of multi-thread
