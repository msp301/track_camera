#ifndef SAFETHREAD_HPP
#define SAFETHREAD_HPP

#include <QObject>
#include <QMutex>

class SafeThread
{
    public:
        SafeThread();
        ~SafeThread();
        void stopThread();
        bool isStopped();
    private:
        bool stop_thread;
        QMutex *thread_state;
};

#endif // SAFETHREAD_HPP
