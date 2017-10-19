#ifndef MLT_UTIL_H
#define MLT_UTIL_H

#ifndef _WIN32
#include <unistd.h>
#endif

#include <QMutex>
#include <QWaitCondition>

#include <framework/mlt_types.h>

typedef QMutex* MLT_MUTEX;
typedef QWaitCondition* MLT_COND;

#define mlt_mutex_init(pMutex)\
{\
    (pMutex) = new QMutex();\
}

#define mlt_mutex_destroy(pMutex)\
{\
    delete (pMutex);\
    (pMutex) = NULL;\
}

#define mlt_mutex_lock(pMutex)\
{\
    (pMutex)->lock();\
}

#define mlt_mutex_unlock(pMutex)\
{\
    (pMutex)->unlock();\
}
#define mlt_cond_init(pCond) \
{\
    (pCond) = new QWaitCondition();\
}

#define mlt_cond_destroy(pCond) \
{\
    delete (pCond);\
    (pCond) = NULL;\
}

#define mlt_cond_wait(pCond, pMutex) \
{\
    (pCond)->wait((pMutex));\
}

#define mlt_cond_timedwait(pCond, pMutex, ms) \
{\
    (pCond)->wait((pMutex), ms);\
}

#define mlt_cond_broadcast(pCond) \
{\
    (pCond)->wakeAll();\
}

#define mlt_cond_signal(pCond) \
{\
    pCond->wakeOne();\
}

#if 0
#include <pthread.h>
#include <sys/time.h>
#define MLT_MUTEX pthread_mutex_t
#define MLT_COND pthread_cond_t

#define mlt_mutex_init(mutex)\
{\
    pthread_mutex_init(&(mutex), NULL);\
}

#define mlt_mutex_destroy(mutex)\
{\
    pthread_mutex_destroy(&(mutex));\
}

#define mlt_mutex_lock(mutex)\
{\
    pthread_mutex_lock(&(mutex));\
}

#define mlt_mutex_unlock(mutex)\
{\
    pthread_mutex_unlock(&(mutex));\
}
#define mlt_cond_init(cond) \
{\
    pthread_cond_init( &(cond), NULL );\
}

#define mlt_cond_destroy(cond) \
{\
    pthread_cond_destroy( &(cond) );\
}

#define mlt_cond_wait(cond, mutex) \
{\
    pthread_cond_wait( &(cond), &(mutex) );\
}

#define mlt_cond_timedwait(cond, mutex, tm) \
{\
    pthread_cond_timedwait( &(cond), &(mutex), &(tm) );\
}

#define mlt_cond_broadcast(cond) \
{\
    pthread_cond_broadcast( &(cond) );\
}
#endif

#ifdef _WIN32
#include <windows.h>
#define MLT_TIMER FILETIME
void timer_start(FILETIME *start);
int64_t timer_elapsed_msec(FILETIME *start);
int64_t timer_elapsed_usec(FILETIME *start);
#else
#define MLT_TIMER struct timeval
void timer_start(struct timeval *stime);
int64_t timer_elapsed_msec(struct timeval *start);
int64_t timer_elapsed_usec(struct timeval *start);
#endif


#ifdef _WIN32
#define MLT_SLEEP_MS(x) Sleep(x)
#else
#define MLT_SLEEP_MS(x) usleep((x)*1000)
#endif

#endif

