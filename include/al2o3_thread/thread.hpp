#pragma once
#include "al2o3_platform/platform.h"
#include "al2o3_thread/thread.h"

namespace Thread {

struct Mutex {
  Mutex() { Thread_MutexCreate(&handle); };
  ~Mutex() { Thread_MutexDestroy(&handle); };

  // take ownership of a C level mutex
  explicit Mutex(Thread_Mutex mutie) : handle(mutie) {};

  void Acquire() { Thread_MutexAcquire(&handle); };
  void Release() { Thread_MutexRelease(&handle); };

	Thread_Mutex handle;
};

struct ConditionalVariable {
  ConditionalVariable() { Thread_ConditionalVariableCreate(&handle); };
  ~ConditionalVariable() { Thread_ConditionalVariableDestroy(&handle); };

  void Wait(Mutex& mutex, uint64_t waitms) {
		Thread_ConditionalVariableWait(&handle, &mutex.handle, waitms);
  }
  void Set() { Thread_ConditionalVariableSet(&handle); };

	Thread_ConditionalVariable handle;
};

struct MutexLock {
  MutexLock(Mutex& mutex) : mMutex(&mutex.handle) { Thread_MutexAcquire(mMutex); };
  MutexLock(Thread_Mutex *mutex) : mMutex(mutex) { Thread_MutexAcquire(mMutex); };
  ~MutexLock() { Thread_MutexRelease(mMutex); };

  /// Prevent copy construction.
  MutexLock(const MutexLock& rhs) = delete;
  /// Prevent assignment.
  MutexLock& operator=(const MutexLock& rhs) = delete;

	Thread_Mutex *mMutex;
};

struct Thread {
  Thread(Thread_JobFunction function, void *data) { Thread_ThreadCreate(&handle, function, data); }
  ~Thread() { Thread_ThreadDestroy(&handle); }

  void Join() { Thread_ThreadJoin(&handle); }

  static void SetMainThread() { Thread_SetMainThread(); };
  static Thread_ThreadID GetCurrentThreadID() { return Thread_GetCurrentThreadID(); };
  static bool IsMainThread() { return Thread_IsMainThread(); }
  static void Sleep(uint64_t waitms) { Thread_Sleep(waitms); }
  static uint32_t GetNumCPUCores(void) { return Thread_CPUCoreCount(); };

	Thread_Thread handle;
};

}; // end Thread namespace
