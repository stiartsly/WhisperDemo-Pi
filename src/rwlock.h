#ifndef __RWLOCK_H__
#define __RWLOCK_H__

#include <memory>
#include <pthread.h>

class CRwLock {
public:
    CRwLock();
    ~CRwLock();

    int rdlock();
    int wrlock();
    int unlock();

private:
    pthread_rwlock_t mLock;
};

class CAutoReadLock {

public:
    CAutoReadLock(std::shared_ptr<CRwLock> lock) {
        mLock = lock;
        mLock->rdlock();
    }

    ~CAutoReadLock() {
        mLock->unlock();
    }

private:
    std::shared_ptr<CRwLock> mLock;
};

class CAutoWriteLock {

public:
    CAutoWriteLock(std::shared_ptr<CRwLock> lock) {
        mLock = lock;
        mLock->wrlock();
    }

    ~CAutoWriteLock() {
        mLock->unlock();
    }

private:
    std::shared_ptr<CRwLock> mLock;
};

#endif

