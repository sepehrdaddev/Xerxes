#ifndef XERXES_SERVERTHREAD_H
#define XERXES_SERVERTHREAD_H

#include <pthread.h>

class ServerThread {
public:
    pthread_t id;

private:
    static pthread_mutex_t mutex;

public:
    ServerThread();
    explicit ServerThread(void *Callback, void *args);
    int Create(void *Callback, void *args);
    int Join();
    int Detach();

    static int InitMutex();
    static int LockMutex();
    static int UnlockMutex();
};


#endif //XERXES_SERVERTHREAD_H
