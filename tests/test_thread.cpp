#include "al2o3_catch2/catch2.hpp"
#include "al2o3_thread/thread.h"

static void TestJob(void* data) {
	REQUIRE(((uint64_t)data) == 10);
	REQUIRE(!Thread_IsMainThread());

}

TEST_CASE("Basic tests", "[al2o3 thread]") {

	Thread_SetMainThread();
	REQUIRE(Thread_IsMainThread());

	Thread_Mutex mutie;

	REQUIRE(Thread_MutexCreate(&mutie));
	Thread_MutexAcquire(&mutie);
	Thread_MutexRelease(&mutie);
	Thread_MutexDestroy(&mutie);

	Thread_ConditionalVariable cv;
	REQUIRE(Thread_ConditionalVariableCreate(&cv));
	Thread_ConditionalVariableDestroy(&cv);

	Thread_Thread testThread;
	REQUIRE( Thread_ThreadCreate(&testThread, &TestJob, (void*)10) );
	Thread_ThreadJoin(&testThread);
	Thread_ThreadDestroy(&testThread);

	Thread_CPUCoreCount();
	Thread_Sleep(10);
}
