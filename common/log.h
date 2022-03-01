#ifndef __LEDED_LOG_H__
#define __LEDED_LOG_H__

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>


#ifndef ARRAY_SIZE
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif

enum {
        L_CRIT,
        L_WARNING,
        L_NOTICE,
        L_INFO,
        L_DEBUG
};

#define DEFAULT_LOG_LEVEL L_INFO

void syslog_init(const char *ident, bool enable, int log_level);
void syslog_message(int priority, const char *format, ...);
void syslog_close(void);

static inline void no_debug(int level, const char *fmt, ...)
{
}

void syslog_message(int priority, const char *format, ...);

#ifdef DEBUG
#define DPRINTF(format, ...) if (1) {\
          syslog_message(L_DEBUG, "%s:%d: "format, __func__, __LINE__, ## __VA_ARGS__);\
        }
#else
#define DPRINTF(format, ...) no_debug(0, format, ## __VA_ARGS__)
#endif

#endif
