#include "al2o3_platform/platform.h"
#include "al2o3_platform/windows.h"
#include "al2o3_os/thread.h"
#include <stdlib.h>
#include "al2o3_memory/memory.h"

static_assert(sizeof(CRITICAL_SECTION) == sizeof(Thread_Mutex), "Mutex size failure in windows/thread.c");
static_assert(sizeof(CONDITION_VARIABLE) == sizeof(Thread_ConditionalVariable), "Condition Variable size failure in windows/thread.c");

AL2O3_EXTERN_C bool Thread_MutexCreate(Thread_Mutex *mutex) {
  InitializeCriticalSection((CRITICAL_SECTION *) mutex);
  return true;
}
AL2O3_EXTERN_C void Thread_MutexDestroy(Thread_Mutex *mutex) {
  DeleteCriticalSection((CRITICAL_SECTION *) mutex);
}

AL2O3_EXTERN_C void Thread_MutexAcquire(Thread_Mutex *mutex) {
  EnterCriticalSection((CRITICAL_SECTION *) mutex);
}
AL2O3_EXTERN_C void Thread_MutexRelease(Thread_Mutex *mutex) {
  LeaveCriticalSection((CRITICAL_SECTION *) mutex);
}

AL2O3_EXTERN_C bool Thread_ConditionalVariableCreate(Thread_ConditionalVariable *cv) {
  InitializeConditionVariable((CONDITION_VARIABLE *) cv);
  return true;
}

AL2O3_EXTERN_C void Thread_ConditionalVariableDestroy(Thread_ConditionalVariable *cv) {
}

AL2O3_EXTERN_C void Thread_ConditionalVariableWait(Thread_ConditionalVariable *cv, Thread_Mutex *mutex, uint64_t waitms) {
  SleepConditionVariableCS((CONDITION_VARIABLE *) cv, (CRITICAL_SECTION *) mutex, (DWORD) waitms);
}
AL2O3_EXTERN_C void Thread_ConditionalVariableSet(Thread_ConditionalVariable *cv) {
  WakeConditionVariable((CONDITION_VARIABLE *) cv);
}

struct TrampParam {
	Thread_JobFunction func;
  void *param;
};

static DWORD WINAPI FuncTrampoline(void *param) {
  struct TrampParam *tp = (struct TrampParam *) param;
  tp->func(tp->param);
  MEMORY_FREE(tp);
  return 0;
}

AL2O3_EXTERN_C bool Thread_ThreadCreate(Thread_Thread *thread, Thread_JobFunction func, void *data) {
  ASSERT(thread);
  struct TrampParam *tp = (struct TrampParam *) MEMORY_MALLOC(sizeof(struct TrampParam));
  tp->func = func;
  tp->param = data;
  *thread = CreateThread(0, 0, &FuncTrampoline, tp, 0, 0);
  
  return (*thread != NULL);
}

AL2O3_EXTERN_C void Thread_ThreadDestroy(Thread_Thread *thread) {
  ASSERT(thread);
  WaitForSingleObject((HANDLE) *thread, INFINITE);
  CloseHandle((HANDLE) *thread);
}
AL2O3_EXTERN_C void Thread_ThreadJoin(Thread_Thread *thread) {
  WaitForSingleObject((HANDLE) *thread, INFINITE);
}

static bool s_isMainThreadIDSet = false;
static Thread_ThreadID s_mainThreadID;

AL2O3_EXTERN_C Thread_ThreadID Thread_GetCurrentThreadID(void) {
  return GetCurrentThreadId();
}
AL2O3_EXTERN_C void Thread_SetMainThread(void) {
  ASSERT(s_isMainThreadIDSet == false);
  s_mainThreadID = GetCurrentThreadId();
  s_isMainThreadIDSet = true;
}

AL2O3_EXTERN_C bool Thread_IsMainThread(void) {
  ASSERT(s_isMainThreadIDSet == true);
  return Thread_GetCurrentThreadID() == s_mainThreadID;
}

AL2O3_EXTERN_C void Thread_Sleep(uint64_t waitms) {
  Sleep((DWORD) waitms);
}
AL2O3_EXTERN_C uint32_t Thread_CPUCoreCount(void) {
  SYSTEM_INFO systemInfo;
  GetSystemInfo(&systemInfo);
  return systemInfo.dwNumberOfProcessors;
}