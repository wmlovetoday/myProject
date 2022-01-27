
#include <sys/time.h>

#define DATA_FOR_ "%d:%d:%d "
#define TIME_FOR_ "%d:%d:%d %ld "
#define DATA_ARG_(ptm) ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday,
#define TIME_PARA_(ptm, tv) ptm->tm_hour, ptm->tm_min, ptm->tm_sec, tv.tv_usec

#define FORMAT__ DATA_FOR_ TIME_FOR_
#define PARA__(ptm, tv) \
    DATA_ARG_(ptm)      \
    TIME_PARA_(ptm, tv)

#define S_FORMAT__ TIME_FOR_
#define S_PARA__(ptm, tv) TIME_PARA_(ptm, tv)

#define PRINT(format, ...)                                                                         \
    do                                                                                             \
    {                                                                                              \
        struct timeval tv;                                                                         \
        gettimeofday(&tv, NULL);                                                                   \
        struct tm *tm = localtime(&(tv.tv_sec));                                                   \
        printf(S_FORMAT__ format " %s:%d\n", S_PARA__(tm, tv), ##__VA_ARGS__, __func__, __LINE__); \
        fflush(stdout);                                                                            \
    } while (0)

#define PRINT_ERRNO(format, ...)                                                                                 \
    do                                                                                                           \
    {                                                                                                            \
        struct timeval tv;                                                                                       \
        gettimeofday(&tv, NULL);                                                                                 \
        struct tm *tm = localtime(&(tv.tv_sec));                                                                 \
        printf(                                                                                                  \
            S_FORMAT__ format " syserr:%s:%d, %s:%d\n", S_PARA__(tm, tv), ##__VA_ARGS__, strerror(errno), errno, \
            __FILE__, __LINE__);                                                                                 \
        fflush(stdout);                                                                                          \
    } while (0)
