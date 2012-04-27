#include "SafeThread.hpp"

#include <QMutexLocker>

SafeThread::SafeThread()
{
    stop_thread = false; //set thread state to run
    thread_state = new QMutex; //create thread state mutex
}

SafeThread::~SafeThread()
{
    delete thread_state; //remove thread state mutex
}

//safely stop thread execution
void SafeThread::stopThread()
{
    QMutexLocker lock( thread_state );
    stop_thread = true; //set thread to be stopped
}

//determine whether thread has been requested to be stopped
bool SafeThread::isStopped()
{
    QMutexLocker lock( thread_state );
    return stop_thread; //return thread execution state
}
