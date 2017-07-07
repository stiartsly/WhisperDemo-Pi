#ifndef __MUTEX_LOCK__
#define __MUTEX_LOCK__

#include <memory>
#include <pthread.h>

class CLock {

public:
    CLock();
    ~CLock();

    int lock(void);
    int unlock(void);

private:
    pthread_mutex_t mLock;
};

class CAutoLock {
public:
    CAutoLock(std::shared_ptr<CLock> lock) {
        mLock = lock;
        mLock->lock();
    }

    ~CAutoLock() {
        mLock->unlock();
    }

private:
    std::shared_ptr<CLock> mLock;
};

#endif

