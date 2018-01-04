#include "serverthread.h"

ServerThread::ServerThread(FrameQueue &frames,unsigned short port):
    frames(frames),
    port(port)
{
    moveToThread(this);
}

void ServerThread::run()
{
    boost::asio::io_service io_service;
    ServerUDP server(io_service,port,frames);
    io_service.run();
}
