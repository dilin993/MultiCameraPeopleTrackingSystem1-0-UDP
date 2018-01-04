#ifndef SERVERTHREAD_H
#include <QThread>
#include "ServerUDP.h"


class ServerThread : public QThread
{
public:
    ServerThread( FrameQueue &frames, unsigned short port);
    void run();

private:
    FrameQueue &frames;
    unsigned short port;
};

#endif // SERVERTHREAD_H
