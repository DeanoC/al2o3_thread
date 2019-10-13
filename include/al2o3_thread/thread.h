#pragma once
#include "al2o3_platform/platform.h"

#if AL2O3_PLATFORM == AL2O3_PLATFORM_WINDOWS

#if AL2O3_CPU_BIT_SIZE == 32
typedef struct { char dummy[24]; } Thread_Mutex;
typedef struct { char dummy[4]; } Thread_ConditionalVariable;
#elif AL2O3_CPU_BIT_SIZE == 64
typedef struct { char dummy[40]; } Thread_Mutex;
typedef struct { char dummy[8]; } Thread_ConditionalVariable;
#else
#error What bit size if this CPU?!
#endif

typedef unsigned int Thread_ID;
typedef void* Thread_Thread;

#else

#include <pthread.h>

/// Operating system mutual exclusion primitive.
typedef pthread_mutex_t Thread_Mutex;
typedef pthread_cond_t Thread_ConditionalVariable;

typedef pthread_t Thread_ThreadID;
typedef pthread_t Thread_Thread;

#endif

typedef void (*Thread_JobFunction)(void *);

AL2O3_EXTERN_C bool Thread_MutexCreate(Thread_Mutex *mutex);
AL2O3_EXTERN_C void Thread_MutexDestroy(Thread_Mutex *mutex);
AL2O3_EXTERN_C void Thread_MutexAcquire(Thread_Mutex *mutex);
AL2O3_EXTERN_C void Thread_MutexRelease(Thread_Mutex *mutex);
AL2O3_EXTERN_C bool Thread_ConditionalVariableCreate(Thread_ConditionalVariable *cd);
AL2O3_EXTERN_C void Thread_ConditionalVariableDestroy(Thread_ConditionalVariable *cd);
AL2O3_EXTERN_C void Thread_ConditionalVariableWait(Thread_ConditionalVariable *cd, Thread_Mutex *mutex, uint64_t waitms);
AL2O3_EXTERN_C void Thread_ConditionalVariableSet(Thread_ConditionalVariable *cd);

AL2O3_EXTERN_C bool Thread_ThreadCreate(Thread_Thread *thread, Thread_JobFunction func, void *data);
AL2O3_EXTERN_C void Thread_ThreadDestroy(Thread_Thread *thread);
AL2O3_EXTERN_C void Thread_ThreadJoin(Thread_Thread *thread);

AL2O3_EXTERN_C Thread_ThreadID Thread_GetCurrentThreadID(void);
AL2O3_EXTERN_C void Thread_SetMainThread(void);
AL2O3_EXTERN_C bool Thread_IsMainThread(void);

AL2O3_EXTERN_C void Thread_Sleep(uint64_t waitms);
// Note in theory this can change at runtime on some platforms
AL2O3_EXTERN_C uint32_t Thread_CPUCoreCount(void);
