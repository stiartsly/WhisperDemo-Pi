#ifndef __VLOG_H__
#define __VLOG_H__

#include <stdarg.h>

#if 0
#ifdef __cplusplus
extern "C" {
#endif
#endif

#define VLOG_ERR        0
#define VLOG_WARN       1
#define VLOG_INFO       2
#define VLOG_DEBUG      3

extern int logLevel;

#define vlogE(format, ...) \
    do { \
        if (logLevel >= VLOG_ERR) \
            logMsg(VLOG_ERR, format, ##__VA_ARGS__); \
    } while(0)

#define vlogW(format, ...) \
    do { \
        if (logLevel >= VLOG_WARN) \
            logMsg(VLOG_WARN, format, ##__VA_ARGS__); \
    } while(0)

#define vlogI(format, ...) \
    do { \
        if (logLevel >= VLOG_INFO) \
            logMsg(VLOG_INFO, format, ##__VA_ARGS__); \
    } while(0)

#define vlogD(format, ...) \
    do { \
        if (logLevel >= VLOG_DEBUG) \
            logMsg(VLOG_DEBUG, format, ##__VA_ARGS__); \
    } while(0)

    

void logMsg(int level, const char *format, ...);
void logMsg(int level, const char *format, va_list args);
void setLogLevel(int level);

#if 0
#ifdef __cplusplus
}
#endif
#endif
#endif
