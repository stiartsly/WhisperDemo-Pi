#include <pthread.h>
#include "rwlock.h"

CRwLock::CRwLock()
{
    pthread_rwlock_init(&mLock, NULL);
}

CRwLock::~CRwLock()
{
    pthread_rwlock_destroy(&mLock);
}

int CRwLock::rdlock(void)
{
    return pthread_rwlock_rdlock(&mLock);
}

int CRwLock::wrlock(void)
{
    return pthread_rwlock_wrlock(&mLock);
}

int CRwLock::unlock(void)
{
    return pthread_rwlock_unlock(&mLock);
}

