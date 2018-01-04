//
// Created by dilin on 11/18/17.
//

#include "FrameQueue.h"

bool FrameQueue::is_empty()
{
    qmutex.lock();
    bool e = frames.empty();
    qmutex.unlock();
    return e;
}

void FrameQueue::enqueue(Frame frame)
{
    qmutex.lock();
    frames.push(frame);
    qmutex.unlock();
}

Frame FrameQueue::dequeue()
{
    qmutex.lock();
    Frame frame = frames.front();
    frames.pop();
    qmutex.unlock();
    return frame;
}

FrameQueue::FrameQueue() :
frames()
{
}
