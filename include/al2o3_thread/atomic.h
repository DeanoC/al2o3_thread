#pragma once
/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing
  Modfied (c) 2019 DeanoC for al2o3

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/

#include "al2o3_platform/platform.h"
#include <stddef.h>

#if AL2O3_COMPILER == AL2O3_COMPILER_MSVC
#include "al2o3_thread/impl/atomic_msvc.h"
#elif AL2O3_COMPILER == AL2O3_COMPILER_GCC || AL2O3_COMPILER == AL2O3_COMPILER_CLANG
#include "al2o3_thread/impl/atomic_gcc_x64.h"
#else
#error "*** Don't have a implementation for this CPU family ***"
#endif

typedef enum {
	Thread_MEMORY_ORDER_RELAXED,
	Thread_MEMORY_ORDER_ACQUIRE,
	Thread_MEMORY_ORDER_RELEASE,
	Thread_MEMORY_ORDER_ACQ_REL
} Thread_memoryOrder_t;

//--------------------------------------------------------------
//  Wrappers for 8-bit operations with built-in ordering constraints
//--------------------------------------------------------------
AL2O3_FORCE_INLINE uint8_t Thread_AtomicLoad8(const Thread_Atomic8_t *object, Thread_memoryOrder_t memoryOrder) {
	uint8_t result = Thread_AtomicLoad8Relaxed(object);
	if (memoryOrder == Thread_MEMORY_ORDER_ACQUIRE || memoryOrder == Thread_MEMORY_ORDER_ACQ_REL) // a little forgiving
			Thread_AtomicThreadFenceAcquire();
	return result;
}
AL2O3_FORCE_INLINE void Thread_AtomicStore8(Thread_Atomic8_t *object, uint8_t desired, Thread_memoryOrder_t memoryOrder) {
	if (memoryOrder == Thread_MEMORY_ORDER_RELEASE || memoryOrder == Thread_MEMORY_ORDER_ACQ_REL) // a little forgiving
			Thread_AtomicThreadFenceRelease();
	Thread_AtomicStore8Relaxed(object, desired);
}
AL2O3_FORCE_INLINE uint8_t Thread_AtomicCompareExchange8(Thread_Atomic8_t *object,
																												 uint8_t expected,
																												 uint8_t desired,
																												 Thread_memoryOrder_t memoryOrder) {
	if (memoryOrder == Thread_MEMORY_ORDER_RELEASE || memoryOrder == Thread_MEMORY_ORDER_ACQ_REL)
			Thread_AtomicThreadFenceRelease();
	uint8_t result = Thread_AtomicCompareExchange8Relaxed(object, expected, desired);
	if (memoryOrder == Thread_MEMORY_ORDER_ACQUIRE || memoryOrder == Thread_MEMORY_ORDER_ACQ_REL)
			Thread_AtomicThreadFenceAcquire();
	return result;
}
AL2O3_FORCE_INLINE intptr_t Thread_AtomicCompareExchangeWeak8(Thread_Atomic8_t *object,
																											uint8_t *expected,
																											uint8_t desired,
																											int success,
																											int failure) {
	if ((success == Thread_MEMORY_ORDER_RELEASE || success == Thread_MEMORY_ORDER_ACQ_REL) ||
			(failure == Thread_MEMORY_ORDER_RELEASE || failure == Thread_MEMORY_ORDER_ACQ_REL)) {
		Thread_AtomicThreadFenceRelease();
	}
	intptr_t result = Thread_AtomicCompareExchangeWeak8Relaxed(object, expected, desired);
	if (result) {
		if (success == Thread_MEMORY_ORDER_ACQUIRE || success == Thread_MEMORY_ORDER_ACQ_REL)
				Thread_AtomicThreadFenceAcquire();
	} else {
		if (failure == Thread_MEMORY_ORDER_ACQUIRE || failure == Thread_MEMORY_ORDER_ACQ_REL)
				Thread_AtomicThreadFenceAcquire();
	}
	return result;
}

AL2O3_FORCE_INLINE uint8_t Thread_AtomicExchange8(Thread_Atomic8_t *object, uint8_t desired, Thread_memoryOrder_t memoryOrder) {
	if (memoryOrder == Thread_MEMORY_ORDER_RELEASE || memoryOrder == Thread_MEMORY_ORDER_ACQ_REL)
			Thread_AtomicThreadFenceRelease();
	uint8_t result = Thread_AtomicExchange8Relaxed(object, desired);
	if (memoryOrder == Thread_MEMORY_ORDER_ACQUIRE || memoryOrder == Thread_MEMORY_ORDER_ACQ_REL)
			Thread_AtomicThreadFenceAcquire();
	return result;
}
AL2O3_FORCE_INLINE uint8_t Thread_AtomicFetchAdd8(Thread_Atomic8_t *object, int8_t operand, Thread_memoryOrder_t memoryOrder) {
	if (memoryOrder == Thread_MEMORY_ORDER_RELEASE || memoryOrder == Thread_MEMORY_ORDER_ACQ_REL)
			Thread_AtomicThreadFenceRelease();
	uint8_t result = Thread_AtomicFetchAdd8Relaxed(object, operand);
	if (memoryOrder == Thread_MEMORY_ORDER_ACQUIRE || memoryOrder == Thread_MEMORY_ORDER_ACQ_REL)
			Thread_AtomicThreadFenceAcquire();
	return result;
}
AL2O3_FORCE_INLINE uint8_t Thread_AtomicFetchAnd8(Thread_Atomic8_t *object, uint8_t operand, Thread_memoryOrder_t memoryOrder) {
	if (memoryOrder == Thread_MEMORY_ORDER_RELEASE || memoryOrder == Thread_MEMORY_ORDER_ACQ_REL)
			Thread_AtomicThreadFenceRelease();
	uint8_t result = Thread_AtomicFetchAnd8Relaxed(object, operand);
	if (memoryOrder == Thread_MEMORY_ORDER_ACQUIRE || memoryOrder == Thread_MEMORY_ORDER_ACQ_REL)
			Thread_AtomicThreadFenceAcquire();
	return result;
}
AL2O3_FORCE_INLINE uint8_t Thread_AtomicFetchOr8(Thread_Atomic8_t *object, uint8_t operand, Thread_memoryOrder_t memoryOrder) {
	if (memoryOrder == Thread_MEMORY_ORDER_RELEASE || memoryOrder == Thread_MEMORY_ORDER_ACQ_REL)
			Thread_AtomicThreadFenceRelease();
	uint8_t result = Thread_AtomicFetchOr8Relaxed(object, operand);
	if (memoryOrder == Thread_MEMORY_ORDER_ACQUIRE || memoryOrder == Thread_MEMORY_ORDER_ACQ_REL)
			Thread_AtomicThreadFenceAcquire();
	return result;
}

//--------------------------------------------------------------
//  Wrappers for 16-bit operations with built-in ordering constraints
//--------------------------------------------------------------
AL2O3_FORCE_INLINE uint16_t Thread_AtomicLoad16(const Thread_Atomic16_t *object, Thread_memoryOrder_t memoryOrder) {
	uint16_t result = Thread_AtomicLoad16Relaxed(object);
	if (memoryOrder == Thread_MEMORY_ORDER_ACQUIRE || memoryOrder == Thread_MEMORY_ORDER_ACQ_REL) // a little forgiving
			Thread_AtomicThreadFenceAcquire();
	return result;
}
AL2O3_FORCE_INLINE void Thread_AtomicStore16(Thread_Atomic16_t *object, uint16_t desired, Thread_memoryOrder_t memoryOrder) {
	if (memoryOrder == Thread_MEMORY_ORDER_RELEASE || memoryOrder == Thread_MEMORY_ORDER_ACQ_REL) // a little forgiving
			Thread_AtomicThreadFenceRelease();
	Thread_AtomicStore16Relaxed(object, desired);
}
AL2O3_FORCE_INLINE uint16_t Thread_AtomicCompareExchange16(Thread_Atomic16_t *object,
																													 uint16_t expected,
																													 uint16_t desired,
																													 Thread_memoryOrder_t memoryOrder) {
	if (memoryOrder == Thread_MEMORY_ORDER_RELEASE || memoryOrder == Thread_MEMORY_ORDER_ACQ_REL)
			Thread_AtomicThreadFenceRelease();
	uint16_t result = Thread_AtomicCompareExchange16Relaxed(object, expected, desired);
	if (memoryOrder == Thread_MEMORY_ORDER_ACQUIRE || memoryOrder == Thread_MEMORY_ORDER_ACQ_REL)
			Thread_AtomicThreadFenceAcquire();
	return result;
}
AL2O3_FORCE_INLINE intptr_t Thread_AtomicCompareExchangeWeak16(Thread_Atomic16_t *object,
																											 uint16_t *expected,
																											 uint16_t desired,
																											 int success,
																											 int failure) {
	if ((success == Thread_MEMORY_ORDER_RELEASE || success == Thread_MEMORY_ORDER_ACQ_REL) ||
			(failure == Thread_MEMORY_ORDER_RELEASE || failure == Thread_MEMORY_ORDER_ACQ_REL))
			Thread_AtomicThreadFenceRelease();
	intptr_t result = Thread_AtomicCompareExchangeWeak16Relaxed(object, expected, desired);
	if (result) {
		if (success == Thread_MEMORY_ORDER_ACQUIRE || success == Thread_MEMORY_ORDER_ACQ_REL)
				Thread_AtomicThreadFenceAcquire();
	} else {
		if (failure == Thread_MEMORY_ORDER_ACQUIRE || failure == Thread_MEMORY_ORDER_ACQ_REL)
				Thread_AtomicThreadFenceAcquire();
	}
	return result;
}
AL2O3_FORCE_INLINE uint16_t Thread_AtomicExchange16(Thread_Atomic16_t *object, uint16_t desired, Thread_memoryOrder_t memoryOrder) {
	if (memoryOrder == Thread_MEMORY_ORDER_RELEASE || memoryOrder == Thread_MEMORY_ORDER_ACQ_REL)
			Thread_AtomicThreadFenceRelease();
	uint16_t result = Thread_AtomicExchange16Relaxed(object, desired);
	if (memoryOrder == Thread_MEMORY_ORDER_ACQUIRE || memoryOrder == Thread_MEMORY_ORDER_ACQ_REL)
			Thread_AtomicThreadFenceAcquire();
	return result;
}
AL2O3_FORCE_INLINE uint16_t Thread_AtomicFetchAdd16(Thread_Atomic16_t *object, int16_t operand, Thread_memoryOrder_t memoryOrder) {
	if (memoryOrder == Thread_MEMORY_ORDER_RELEASE || memoryOrder == Thread_MEMORY_ORDER_ACQ_REL)
			Thread_AtomicThreadFenceRelease();
	uint16_t result = Thread_AtomicFetchAdd16Relaxed(object, operand);
	if (memoryOrder == Thread_MEMORY_ORDER_ACQUIRE || memoryOrder == Thread_MEMORY_ORDER_ACQ_REL)
			Thread_AtomicThreadFenceAcquire();
	return result;
}
AL2O3_FORCE_INLINE uint16_t Thread_AtomicFetchAnd16(Thread_Atomic16_t *object, uint16_t operand, Thread_memoryOrder_t memoryOrder) {
	if (memoryOrder == Thread_MEMORY_ORDER_RELEASE || memoryOrder == Thread_MEMORY_ORDER_ACQ_REL)
			Thread_AtomicThreadFenceRelease();
	uint16_t result = Thread_AtomicFetchAnd16Relaxed(object, operand);
	if (memoryOrder == Thread_MEMORY_ORDER_ACQUIRE || memoryOrder == Thread_MEMORY_ORDER_ACQ_REL)
			Thread_AtomicThreadFenceAcquire();
	return result;
}
AL2O3_FORCE_INLINE uint16_t Thread_AtomicFetchOr16(Thread_Atomic16_t *object, uint16_t operand, Thread_memoryOrder_t memoryOrder) {
	if (memoryOrder == Thread_MEMORY_ORDER_RELEASE || memoryOrder == Thread_MEMORY_ORDER_ACQ_REL)
			Thread_AtomicThreadFenceRelease();
	uint16_t result = Thread_AtomicFetchOr16Relaxed(object, operand);
	if (memoryOrder == Thread_MEMORY_ORDER_ACQUIRE || memoryOrder == Thread_MEMORY_ORDER_ACQ_REL)
			Thread_AtomicThreadFenceAcquire();
	return result;
}

//--------------------------------------------------------------
//  Wrappers for 32-bit operations with built-in ordering constraints
//--------------------------------------------------------------
AL2O3_FORCE_INLINE uint32_t Thread_AtomicLoad32(const Thread_Atomic32_t *object, Thread_memoryOrder_t memoryOrder) {
	uint32_t result = Thread_AtomicLoad32Relaxed(object);
	if (memoryOrder == Thread_MEMORY_ORDER_ACQUIRE || memoryOrder == Thread_MEMORY_ORDER_ACQ_REL) // a little forgiving
			Thread_AtomicThreadFenceAcquire();
	return result;
}
AL2O3_FORCE_INLINE void Thread_AtomicStore32(Thread_Atomic32_t *object, uint32_t desired, Thread_memoryOrder_t memoryOrder) {
	if (memoryOrder == Thread_MEMORY_ORDER_RELEASE || memoryOrder == Thread_MEMORY_ORDER_ACQ_REL) // a little forgiving
			Thread_AtomicThreadFenceRelease();
	Thread_AtomicStore32Relaxed(object, desired);
}
AL2O3_FORCE_INLINE uint32_t Thread_AtomicCompareExchange32(Thread_Atomic32_t *object,
																													 uint32_t expected,
																													 uint32_t desired,
																													 Thread_memoryOrder_t memoryOrder) {
	if (memoryOrder == Thread_MEMORY_ORDER_RELEASE || memoryOrder == Thread_MEMORY_ORDER_ACQ_REL)
			Thread_AtomicThreadFenceRelease();
	uint32_t result = Thread_AtomicCompareExchange32Relaxed(object, expected, desired);
	if (memoryOrder == Thread_MEMORY_ORDER_ACQUIRE || memoryOrder == Thread_MEMORY_ORDER_ACQ_REL)
			Thread_AtomicThreadFenceAcquire();
	return result;
}
AL2O3_FORCE_INLINE intptr_t Thread_AtomicCompareExchangeWeak32(Thread_Atomic32_t *object,
																											 uint32_t *expected,
																											 uint32_t desired,
																											 int success,
																											 int failure) {
	if ((success == Thread_MEMORY_ORDER_RELEASE || success == Thread_MEMORY_ORDER_ACQ_REL) ||
			(failure == Thread_MEMORY_ORDER_RELEASE || failure == Thread_MEMORY_ORDER_ACQ_REL))
			Thread_AtomicThreadFenceRelease();
	intptr_t result = Thread_AtomicCompareExchangeWeak32Relaxed(object, expected, desired);
	if (result) {
		if (success == Thread_MEMORY_ORDER_ACQUIRE || success == Thread_MEMORY_ORDER_ACQ_REL)
				Thread_AtomicThreadFenceAcquire();
	} else {
		if (failure == Thread_MEMORY_ORDER_ACQUIRE || failure == Thread_MEMORY_ORDER_ACQ_REL)
				Thread_AtomicThreadFenceAcquire();
	}
	return result;
}
AL2O3_FORCE_INLINE uint32_t Thread_AtomicExchange32(Thread_Atomic32_t *object, uint32_t desired, Thread_memoryOrder_t memoryOrder) {
	if (memoryOrder == Thread_MEMORY_ORDER_RELEASE || memoryOrder == Thread_MEMORY_ORDER_ACQ_REL)
			Thread_AtomicThreadFenceRelease();
	uint32_t result = Thread_AtomicExchange32Relaxed(object, desired);
	if (memoryOrder == Thread_MEMORY_ORDER_ACQUIRE || memoryOrder == Thread_MEMORY_ORDER_ACQ_REL)
			Thread_AtomicThreadFenceAcquire();
	return result;
}
AL2O3_FORCE_INLINE uint32_t Thread_AtomicFetchAdd32(Thread_Atomic32_t *object, int32_t operand, Thread_memoryOrder_t memoryOrder) {
	if (memoryOrder == Thread_MEMORY_ORDER_RELEASE || memoryOrder == Thread_MEMORY_ORDER_ACQ_REL)
			Thread_AtomicThreadFenceRelease();
	uint32_t result = Thread_AtomicFetchAdd32Relaxed(object, operand);
	if (memoryOrder == Thread_MEMORY_ORDER_ACQUIRE || memoryOrder == Thread_MEMORY_ORDER_ACQ_REL)
			Thread_AtomicThreadFenceAcquire();
	return result;
}
AL2O3_FORCE_INLINE uint32_t Thread_AtomicFetchAnd32(Thread_Atomic32_t *object, uint32_t operand, Thread_memoryOrder_t memoryOrder) {
	if (memoryOrder == Thread_MEMORY_ORDER_RELEASE || memoryOrder == Thread_MEMORY_ORDER_ACQ_REL)
			Thread_AtomicThreadFenceRelease();
	uint32_t result = Thread_AtomicFetchAnd32Relaxed(object, operand);
	if (memoryOrder == Thread_MEMORY_ORDER_ACQUIRE || memoryOrder == Thread_MEMORY_ORDER_ACQ_REL)
			Thread_AtomicThreadFenceAcquire();
	return result;
}
AL2O3_FORCE_INLINE uint32_t Thread_AtomicFetchOr32(Thread_Atomic32_t *object, uint32_t operand, Thread_memoryOrder_t memoryOrder) {
	if (memoryOrder == Thread_MEMORY_ORDER_RELEASE || memoryOrder == Thread_MEMORY_ORDER_ACQ_REL)
			Thread_AtomicThreadFenceRelease();
	uint32_t result = Thread_AtomicFetchOr32Relaxed(object, operand);
	if (memoryOrder == Thread_MEMORY_ORDER_ACQUIRE || memoryOrder == Thread_MEMORY_ORDER_ACQ_REL)
			Thread_AtomicThreadFenceAcquire();
	return result;
}

//--------------------------------------------------------------
//  Wrappers for 64-bit operations with built-in ordering constraints
//--------------------------------------------------------------
AL2O3_FORCE_INLINE uint64_t Thread_AtomicLoad64(const Thread_Atomic64_t *object, Thread_memoryOrder_t memoryOrder) {
	uint64_t result = Thread_AtomicLoad64Relaxed(object);
	if (memoryOrder == Thread_MEMORY_ORDER_ACQUIRE || memoryOrder == Thread_MEMORY_ORDER_ACQ_REL) // a little forgiving
			Thread_AtomicThreadFenceAcquire();
	return result;
}
AL2O3_FORCE_INLINE void Thread_AtomicStore64(Thread_Atomic64_t *object, uint64_t desired, Thread_memoryOrder_t memoryOrder) {
	if (memoryOrder == Thread_MEMORY_ORDER_RELEASE || memoryOrder == Thread_MEMORY_ORDER_ACQ_REL) // a little forgiving
			Thread_AtomicThreadFenceRelease();
	Thread_AtomicStore64Relaxed(object, desired);
}
AL2O3_FORCE_INLINE uint64_t Thread_AtomicCompareExchange64(Thread_Atomic64_t *object,
																													 uint64_t expected,
																													 uint64_t desired,
																													 Thread_memoryOrder_t memoryOrder) {
	if (memoryOrder == Thread_MEMORY_ORDER_RELEASE || memoryOrder == Thread_MEMORY_ORDER_ACQ_REL)
			Thread_AtomicThreadFenceRelease();
	uint64_t result = Thread_AtomicCompareExchange64Relaxed(object, expected, desired);
	if (memoryOrder == Thread_MEMORY_ORDER_ACQUIRE || memoryOrder == Thread_MEMORY_ORDER_ACQ_REL)
			Thread_AtomicThreadFenceAcquire();
	return result;
}
AL2O3_FORCE_INLINE intptr_t Thread_AtomicCompareExchangeWeak64(Thread_Atomic64_t *object,
																											 uint64_t *expected,
																											 uint64_t desired,
																											 int success,
																											 int failure) {
	if ((success == Thread_MEMORY_ORDER_RELEASE || success == Thread_MEMORY_ORDER_ACQ_REL) ||
			(failure == Thread_MEMORY_ORDER_RELEASE || failure == Thread_MEMORY_ORDER_ACQ_REL))
			Thread_AtomicThreadFenceRelease();
	intptr_t result = Thread_AtomicCompareExchangeWeak64Relaxed(object, expected, desired);
	if (result) {
		if (success == Thread_MEMORY_ORDER_ACQUIRE || success == Thread_MEMORY_ORDER_ACQ_REL)
				Thread_AtomicThreadFenceAcquire();
	} else {
		if (failure == Thread_MEMORY_ORDER_ACQUIRE || failure == Thread_MEMORY_ORDER_ACQ_REL)
				Thread_AtomicThreadFenceAcquire();
	}
	return result;
}
AL2O3_FORCE_INLINE uint64_t Thread_AtomicExchange64(Thread_Atomic64_t *object, uint64_t desired, Thread_memoryOrder_t memoryOrder) {
	if (memoryOrder == Thread_MEMORY_ORDER_RELEASE || memoryOrder == Thread_MEMORY_ORDER_ACQ_REL)
			Thread_AtomicThreadFenceRelease();
	uint64_t result = Thread_AtomicExchange64Relaxed(object, desired);
	if (memoryOrder == Thread_MEMORY_ORDER_ACQUIRE || memoryOrder == Thread_MEMORY_ORDER_ACQ_REL)
			Thread_AtomicThreadFenceAcquire();
	return result;
}
AL2O3_FORCE_INLINE uint64_t Thread_AtomicFetchAdd64(Thread_Atomic64_t *object, int64_t operand, Thread_memoryOrder_t memoryOrder) {
	if (memoryOrder == Thread_MEMORY_ORDER_RELEASE || memoryOrder == Thread_MEMORY_ORDER_ACQ_REL)
			Thread_AtomicThreadFenceRelease();
	uint64_t result = Thread_AtomicFetchAdd64Relaxed(object, operand);
	if (memoryOrder == Thread_MEMORY_ORDER_ACQUIRE || memoryOrder == Thread_MEMORY_ORDER_ACQ_REL)
			Thread_AtomicThreadFenceAcquire();
	return result;
}
AL2O3_FORCE_INLINE uint64_t Thread_AtomicFetchAnd64(Thread_Atomic64_t *object, uint64_t operand, Thread_memoryOrder_t memoryOrder) {
	if (memoryOrder == Thread_MEMORY_ORDER_RELEASE || memoryOrder == Thread_MEMORY_ORDER_ACQ_REL)
			Thread_AtomicThreadFenceRelease();
	uint64_t result = Thread_AtomicFetchAnd64Relaxed(object, operand);
	if (memoryOrder == Thread_MEMORY_ORDER_ACQUIRE || memoryOrder == Thread_MEMORY_ORDER_ACQ_REL)
			Thread_AtomicThreadFenceAcquire();
	return result;
}
AL2O3_FORCE_INLINE uint64_t Thread_AtomicFetchOr64(Thread_Atomic64_t *object, uint64_t operand, Thread_memoryOrder_t memoryOrder) {
	if (memoryOrder == Thread_MEMORY_ORDER_RELEASE || memoryOrder == Thread_MEMORY_ORDER_ACQ_REL)
			Thread_AtomicThreadFenceRelease();
	uint64_t result = Thread_AtomicFetchOr64Relaxed(object, operand);
	if (memoryOrder == Thread_MEMORY_ORDER_ACQUIRE || memoryOrder == Thread_MEMORY_ORDER_ACQ_REL)
			Thread_AtomicThreadFenceAcquire();
	return result;
}

AL2O3_FORCE_INLINE void *Thread_AtomicLoadPtrRelaxed(const Thread_AtomicPtr_t *object) {
	return (void *) Thread_AtomicLoad64Relaxed((const Thread_Atomic64_t *) object);
}
AL2O3_FORCE_INLINE void Thread_AtomicStorePtrRelaxed(Thread_AtomicPtr_t *object, void *desired) {
	Thread_AtomicStore64Relaxed((Thread_Atomic64_t *) object, (size_t) desired);
}
AL2O3_FORCE_INLINE void *Thread_AtomicCompareExchangePtrRelaxed(Thread_AtomicPtr_t *object, void *expected, void *desired) {
	return (void *) Thread_AtomicCompareExchange64Relaxed((Thread_Atomic64_t *) object, (size_t) expected, (size_t) desired);
}
AL2O3_FORCE_INLINE intptr_t Thread_AtomicCompareExchangeWeakPtrRelaxed(Thread_AtomicPtr_t *object,
																															 void **expected,
																															 void *desired) {
	return Thread_AtomicCompareExchangeWeak64Relaxed((Thread_Atomic64_t *) object, (uint64_t *) expected, (size_t) desired);
}
AL2O3_FORCE_INLINE void *Thread_AtomicExchangePtrRelaxed(Thread_AtomicPtr_t *object, void *desired) {
	return (void *) Thread_AtomicExchange64Relaxed((Thread_Atomic64_t *) object, (size_t) desired);
}
AL2O3_FORCE_INLINE void *Thread_AtomicFetchAddPtrRelaxed(Thread_AtomicPtr_t *object, ptrdiff_t operand) {
	return (void *) Thread_AtomicFetchAdd64Relaxed((Thread_Atomic64_t *) object, operand);
}
AL2O3_FORCE_INLINE void *Thread_AtomicFetchAndPtrRelaxed(Thread_AtomicPtr_t *object, size_t operand) {
	return (void *) Thread_AtomicFetchAnd64Relaxed((Thread_Atomic64_t *) object, operand);
}
AL2O3_FORCE_INLINE void *Thread_AtomicFetchOrPtrRelaxed(Thread_AtomicPtr_t *object, size_t operand) {
	return (void *) Thread_AtomicFetchOr64Relaxed((Thread_Atomic64_t *) object, operand);
}
AL2O3_FORCE_INLINE void *Thread_AtomicLoadPtr(const Thread_AtomicPtr_t *object, Thread_memoryOrder_t memoryOrder) {
	return (void *) Thread_AtomicLoad64((const Thread_Atomic64_t *) object, memoryOrder);
}
AL2O3_FORCE_INLINE void Thread_AtomicStorePtr(Thread_AtomicPtr_t *object, void *desired, Thread_memoryOrder_t memoryOrder) {
	Thread_AtomicStore64((Thread_Atomic64_t *) object, (size_t) desired, memoryOrder);
}
AL2O3_FORCE_INLINE void *Thread_AtomicCompareExchangePtr(Thread_AtomicPtr_t *object,
																												 void *expected,
																												 void *desired,
																												 Thread_memoryOrder_t memoryOrder) {
	return (void *) Thread_AtomicCompareExchange64((Thread_Atomic64_t *) object, (size_t) expected, (size_t) desired, memoryOrder);
}
AL2O3_FORCE_INLINE intptr_t Thread_AtomicCompareExchangeWeakPtr(Thread_AtomicPtr_t *object,
																												void **expected,
																												void *desired,
																												int success,
																												int failure) {
	return Thread_AtomicCompareExchangeWeak64((Thread_Atomic64_t *) object,
																		(uint64_t *) expected,
																		(size_t) desired,
																		success,
																		failure);
}
AL2O3_FORCE_INLINE void *Thread_AtomicExchangePtr(Thread_AtomicPtr_t *object, void *desired, Thread_memoryOrder_t memoryOrder) {
	return (void *) Thread_AtomicExchange64((Thread_Atomic64_t *) object, (size_t) desired, memoryOrder);
}
AL2O3_FORCE_INLINE void *Thread_AtomicFetchAddPtr(Thread_AtomicPtr_t *object, ptrdiff_t operand, Thread_memoryOrder_t memoryOrder) {
	return (void *) Thread_AtomicFetchAdd64((Thread_Atomic64_t *) object, operand, memoryOrder);
}
AL2O3_FORCE_INLINE void *Thread_AtomicFetchAndPtr(Thread_AtomicPtr_t *object, size_t operand, Thread_memoryOrder_t memoryOrder) {
	return (void *) Thread_AtomicFetchAnd64((Thread_Atomic64_t *) object, operand, memoryOrder);
}
AL2O3_FORCE_INLINE void *Thread_AtomicFetchOrPtr(Thread_AtomicPtr_t *object, size_t operand, Thread_memoryOrder_t memoryOrder) {
	return (void *) Thread_AtomicFetchOr64((Thread_Atomic64_t *) object, operand, memoryOrder);
}
