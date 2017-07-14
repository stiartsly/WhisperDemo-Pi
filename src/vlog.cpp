#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <pthread.h>

#include "vlog.h"

#define TIME_FORMAT     "%Y-%m-%d %H:%M:%S"

int logLevel = VLOG_INFO;

static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

static const char* level_names[] = {
    "ERROR",
    "WARNING",
    "INFO",
    "DEBUG"
};

void eclogv(int level, const char *format, va_list args)
{
    if (level > logLevel)
        return;
    
    if (level > VLOG_DEBUG)
        level = VLOG_DEBUG;

    {
        int rc;
        char timestr[20];
        char buf[1024];
        char out[1024];
        time_t cur = time(NULL);

        strftime(timestr, 20, TIME_FORMAT, localtime(&cur));
        rc = vsnprintf(buf, sizeof(buf), format, args);
        if (rc == -1)
            buf[1023] = 0;

        rc = snprintf(out, sizeof(out), "%s - %-7s : %s\n",
                       timestr, level_names[level], buf);
        if (rc == -1)
            out[1023] = 0;

        pthread_mutex_lock(&lock);
        //set_color(level);
        fputs(out, stderr);
        //reset_color();
        pthread_mutex_unlock(&lock);
        fflush(stderr);
    }
}

void logMsg(int level, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    eclogv(level, format, args);
    va_end(args);
}

void logMsgV(int level, const char *format, va_list args)
{
    char out[512];
    int rc;
    (void)level;

    rc = vsnprintf(out, sizeof(out), format, args);
    if (rc == -1)
        out[511] = 0;

    fputs(out, stderr);
    fflush(stderr);
}

void setLogLevel(int level)
{
    if (level > VLOG_DEBUG)
        level = VLOG_DEBUG;

    logLevel = level;
}
