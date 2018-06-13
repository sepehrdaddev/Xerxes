#include "../Headers/ServerThread.hpp"
#include "../Headers/Logging.hpp"

pthread_mutex_t ServerThread::mutex;

ServerThread::ServerThread() = default;

int ServerThread::Create(void *Callback, void *args) {
    int rc;

    rc = pthread_create(&this->id, nullptr, reinterpret_cast<void *(*)(void *)>(Callback), args);

    if(rc) {
        print_error("Error while creating threads.");
    }
    return rc;
}

int ServerThread::Join() {
    if(pthread_join(this->id, nullptr) == -1){
        print_error("Error while joining");
        return -1;
    }
    return 0;
}

int ServerThread::InitMutex() {
    if(pthread_mutex_init(&ServerThread::mutex, nullptr) == -1) {
        print_error("Error while initializing mutex");
        return -1;
    }
    return 0;
}

int ServerThread::LockMutex() {
    if((pthread_mutex_lock(&ServerThread::mutex)) == -1) {
        print_error("Error while locking");
        return -1;
    }
    return 0;
}

int ServerThread::UnlockMutex() {
    if((pthread_mutex_unlock(&ServerThread::mutex)) == -1) {
        print_error("Error while releasing the lock");
        return -1;
    }
    return 0;
}

int ServerThread::Detach() {
    if((pthread_detach(this->id)) == -1){
        print_error("Error while detach");
        return -1;
    }
    return 0;
}
