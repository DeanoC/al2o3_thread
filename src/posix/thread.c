#include "al2o3_platform/platform.h"
#include "al2o3_thread/thread.h"
#include "al2o3_memory/memory.h"
#include <unistd.h>
#include <sys/sysctl.h>
#if defined(__linux__)
#include <sys/sysinfo.h>
#endif
#include <pthread.h>

AL2O3_EXTERN_C bool Thread_MutexCreate(Thread_Mutex *mutex) {
  ASSERT(mutex);
  return pthread_mutex_init(mutex, NULL) == 0;
}

AL2O3_EXTERN_C void Thread_MutexDestroy(Thread_Mutex *mutex) {
  ASSERT(mutex);
  pthread_mutex_destroy(mutex);
}

AL2O3_EXTERN_C void Thread_MutexAcquire(Thread_Mutex *mutex) {
  ASSERT(mutex);
  pthread_mutex_lock(mutex);

}

AL2O3_EXTERN_C void Thread_MutexRelease(Thread_Mutex *mutex) {
  ASSERT(mutex);
  pthread_mutex_unlock(mutex);
}

AL2O3_EXTERN_C bool Thread_ConditionalVariableCreate(Thread_ConditionalVariable *cv) {
  ASSERT(cv);
  return pthread_cond_init(cv, NULL) == 0;
}

AL2O3_EXTERN_C void Thread_ConditionalVariableDestroy(Thread_ConditionalVariable *cv) {
  ASSERT(cv);
  pthread_cond_destroy(cv);
}

AL2O3_EXTERN_C void Thread_ConditionalVariableWait(Thread_ConditionalVariable *cv, Thread_Mutex *mutex, uint64_t waitms) {
  ASSERT(cv);
  ASSERT(mutex);

  struct timespec ts;
  ts.tv_sec = 0;
  ts.tv_nsec = (long) waitms * 1000;

  pthread_mutex_t *mutexHandle = mutex;
  pthread_cond_timedwait(cv, mutexHandle, &ts);

}

AL2O3_EXTERN_C void Thread_ConditionalVariableSet(Thread_ConditionalVariable *cv) {
  ASSERT(cv);
  pthread_cond_signal(cv);
}

struct TrampParam {
	Thread_JobFunction func;
  void *param;
};

static void *FuncTrampoline(void *param) {
  struct TrampParam *tp = (struct TrampParam *) param;
  tp->func(tp->param);
  MEMORY_FREE(tp);

  return NULL;
}

AL2O3_EXTERN_C bool Thread_ThreadCreate(Thread_Thread *thread, Thread_JobFunction func, void *data) {
  ASSERT(thread);
  struct TrampParam *tp = (struct TrampParam *) MEMORY_MALLOC(sizeof(struct TrampParam));
  tp->func = func;
  tp->param = data;

  return pthread_create(thread, NULL, &FuncTrampoline, tp) == 0;
}

AL2O3_EXTERN_C void Thread_ThreadDestroy(Thread_Thread *thread) {
  ASSERT(thread);

  // should this be detach rather than join?
  pthread_join(*thread, NULL);
}

AL2O3_EXTERN_C void Thread_ThreadJoin(Thread_Thread *thread) {
  ASSERT(thread);
  pthread_join(*thread, NULL);
}

AL2O3_EXTERN_C void Thread_Sleep(uint64_t waitms) {
  usleep((useconds_t) waitms * 1000);
}

AL2O3_EXTERN_C uint32_t Thread_CPUCoreCount(void) {
#if defined(__linux__)
 return get_nprocs_conf();
#else
  size_t len;
  unsigned int ncpu;
  len = sizeof(ncpu);
  sysctlbyname("hw.ncpu", &ncpu, &len, NULL, 0);
  return (uint32_t) ncpu;
#endif
}

static bool s_isMainThreadIDSet = false;
static Thread_ThreadID s_mainThreadID;

AL2O3_EXTERN_C void Thread_SetMainThread(void) {
  ASSERT(s_isMainThreadIDSet == false);
  s_mainThreadID = Thread_GetCurrentThreadID();
  s_isMainThreadIDSet = true;
}

AL2O3_EXTERN_C Thread_ThreadID Thread_GetCurrentThreadID(void) {
  return (Thread_ThreadID) pthread_self();
}

AL2O3_EXTERN_C bool Thread_IsMainThread(void) {
  ASSERT(s_isMainThreadIDSet);
  return Thread_GetCurrentThreadID() == s_mainThreadID;
}
