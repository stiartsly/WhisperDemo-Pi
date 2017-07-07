#include <cstdlib>
#include <pthread.h>
#include "lock.h"

CLock::CLock()
{
    pthread_mutex_init(&mLock, NULL);
}

CLock::~CLock()
{
    pthread_mutex_destroy(&mLock);
}

int CLock::lock(void)
{
    return pthread_mutex_lock(&mLock);
}

int CLock::unlock(void)
{
    return pthread_mutex_unlock(&mLock);
}

