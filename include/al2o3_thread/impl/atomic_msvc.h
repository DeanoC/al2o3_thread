#pragma once
/*------------------------------------------------------------------------
  Turf: Configurable C++ platform adapter
  Copyright (c) 2016 Jeff Preshing

  Distributed under the Simplified BSD License.
  Original location: https://github.com/preshing/turf

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the LICENSE file for more information.
------------------------------------------------------------------------*/
#include "al2o3_platform/platform.h"

//-------------------------------------
//  Atomic types
//-------------------------------------
// In MSVC, correct alignment of each type is already ensured.
// MSVC doesn't seem subject to out-of-thin-air stores like GCC, so volatile is
// omitted.
// (MS volatile implies acquire & release semantics, which may be expensive on
// ARM or Xbox 360.)
typedef struct { uint8_t nonatomic; } Thread_Atomic8_t;
typedef __declspec(align(2)) struct { uint16_t nonatomic; } Thread_Atomic16_t;
typedef __declspec(align(4)) struct { uint32_t nonatomic; } Thread_Atomic32_t;
typedef __declspec(align(8)) struct { uint64_t nonatomic; } Thread_Atomic64_t;
typedef __declspec(align(8)) struct { void* nonatomic; } Thread_AtomicPtr_t;

//-------------------------------------
//  Fences
//-------------------------------------
#define Thread_AtomicSignalFenceConsume() (0)
#define Thread_AtomicSignalFenceAcquire() _ReadWriteBarrier()
#define Thread_AtomicSignalFenceRelease() _ReadWriteBarrier()
#define Thread_AtomicSignalFenceSeqCst() _ReadWriteBarrier()

#define Thread_AtomicThreadFenceConsume() (0)
#define Thread_AtomicThreadFenceAcquire() _ReadWriteBarrier()
#define Thread_AtomicThreadFenceRelease() _ReadWriteBarrier()
#define Thread_AtomicThreadFenceSeqCst() MemoryBarrier()

//----------------------------------------------
//  8-bit atomic operations
//----------------------------------------------
AL2O3_FORCE_INLINE uint8_t Thread_AtomicLoad8Relaxed(const Thread_Atomic8_t* object) {
	// Do a volatile load so that compiler doesn't duplicate loads, which makes
	// them nonatomic. (Happened in testing.)
	return ((volatile Thread_Atomic8_t*) object)->nonatomic;
}

AL2O3_FORCE_INLINE void Thread_AtomicStore8Relaxed(Thread_Atomic8_t* object, uint8_t value) {
	((volatile Thread_Atomic8_t*) object)->nonatomic = value;
}

AL2O3_FORCE_INLINE uint8_t Thread_AtomicCompareExchange8Relaxed(Thread_Atomic8_t* object, uint8_t expected, uint8_t desired) {
return _InterlockedCompareExchange8((char*) object, desired, expected);
}

AL2O3_FORCE_INLINE intptr_t Thread_AtomicCompareExchangeWeak8Relaxed(Thread_Atomic8_t* object, uint8_t* expected, uint8_t desired) {
	uint8_t e = *expected;
	uint8_t previous = _InterlockedCompareExchange8((char*) object, desired, e);
	intptr_t matched = (previous == e);
	if (!matched)
	*expected = previous;
	return matched;
}

AL2O3_FORCE_INLINE uint8_t Thread_AtomicExchange8Relaxed(Thread_Atomic8_t* object, uint8_t desired) {
	return _InterlockedExchange8((char*) object, desired);
}

AL2O3_FORCE_INLINE uint8_t Thread_AtomicFetchAdd8Relaxed(Thread_Atomic8_t* object, int8_t operand) {
	return _InterlockedExchangeAdd8((char*) object, operand);
}

AL2O3_FORCE_INLINE uint8_t Thread_AtomicFetchAnd8Relaxed(Thread_Atomic8_t* object, uint8_t operand) {
	return _InterlockedAnd8((char*) object, operand);
}

AL2O3_FORCE_INLINE uint8_t Thread_AtomicFetchOr8Relaxed(Thread_Atomic8_t* object, uint8_t operand) {
	return _InterlockedOr8((char*) object, operand);
}

//----------------------------------------------
//  16-bit atomic operations
//----------------------------------------------
AL2O3_FORCE_INLINE uint16_t Thread_AtomicLoad16Relaxed(const Thread_Atomic16_t* object) {
	// Do a volatile load so that compiler doesn't duplicate loads, which makes
	// them nonatomic. (Happened in testing.)
	return ((volatile Thread_Atomic16_t*) object)->nonatomic;
}

AL2O3_FORCE_INLINE void Thread_AtomicStore16Relaxed(Thread_Atomic16_t* object, uint16_t value) {
	((volatile Thread_Atomic16_t*) object)->nonatomic = value;
}

AL2O3_FORCE_INLINE uint16_t Thread_AtomicCompareExchange16Relaxed(Thread_Atomic16_t* object, uint16_t expected, uint16_t desired) {
	return _InterlockedCompareExchange16((short*) object, desired, expected);
}

AL2O3_FORCE_INLINE intptr_t Thread_AtomicCompareExchangeWeak16Relaxed(Thread_Atomic16_t* object, uint16_t* expected, uint16_t desired) {
	uint16_t e = *expected;
	uint16_t previous = _InterlockedCompareExchange16((short*) object, desired, e);
	intptr_t matched = (previous == e);
	if (!matched)
		*expected = previous;
	return matched;
}

AL2O3_FORCE_INLINE uint16_t Thread_AtomicExchange16Relaxed(Thread_Atomic16_t* object, uint16_t desired) {
	return _InterlockedExchange16((short*) object, desired);
}

AL2O3_FORCE_INLINE uint16_t Thread_AtomicFetchAdd16Relaxed(Thread_Atomic16_t* object, int16_t operand) {
	return _InterlockedExchangeAdd16((short*) object, operand);
}

AL2O3_FORCE_INLINE uint16_t Thread_AtomicFetchAnd16Relaxed(Thread_Atomic16_t* object, uint16_t operand) {
	return _InterlockedAnd16((short*) object, operand);
}

AL2O3_FORCE_INLINE uint16_t Thread_AtomicFetchOr16Relaxed(Thread_Atomic16_t* object, uint16_t operand) {
	return _InterlockedOr16((short*) object, operand);
}

//----------------------------------------------
//  32-bit atomic operations
//----------------------------------------------
AL2O3_FORCE_INLINE uint32_t Thread_AtomicLoad32Relaxed(const Thread_Atomic32_t* object) {
	// Do a volatile load so that compiler doesn't duplicate loads, which makes
	// them nonatomic. (Happened in testing.)
	return ((volatile Thread_Atomic32_t*) object)->nonatomic;
}

AL2O3_FORCE_INLINE void Thread_AtomicStore32Relaxed(Thread_Atomic32_t* object, uint32_t value) {
	((volatile Thread_Atomic32_t*) object)->nonatomic = value;
}

AL2O3_FORCE_INLINE uint32_t Thread_AtomicCompareExchange32Relaxed(Thread_Atomic32_t* object, uint32_t expected, uint32_t desired) {
	return _InterlockedCompareExchange((long*) object, desired, expected);
}

AL2O3_FORCE_INLINE intptr_t Thread_AtomicCompareExchangeWeak32Relaxed(Thread_Atomic32_t* object, uint32_t* expected, uint32_t desired) {
	uint32_t e = *expected;
	uint32_t previous = _InterlockedCompareExchange((long*) object, desired, e);
	intptr_t matched = (previous == e);
	if (!matched)
		*expected = previous;
	return matched;
}

AL2O3_FORCE_INLINE uint32_t Thread_AtomicExchange32Relaxed(Thread_Atomic32_t* object, uint32_t desired) {
	return _InterlockedExchange((long*) object, desired);
}

AL2O3_FORCE_INLINE uint32_t Thread_AtomicFetchAdd32Relaxed(Thread_Atomic32_t* object, int32_t operand) {
	return _InterlockedExchangeAdd((long*) object, operand);
}

AL2O3_FORCE_INLINE uint32_t Thread_AtomicFetchAnd32Relaxed(Thread_Atomic32_t* object, uint32_t operand) {
	return _InterlockedAnd((long*) object, operand);
}

AL2O3_FORCE_INLINE uint32_t Thread_AtomicFetchOr32Relaxed(Thread_Atomic32_t* object, uint32_t operand) {
	return _InterlockedOr((long*) object, operand);
}

//----------------------------------------------
//  64-bit atomic operations
//----------------------------------------------
AL2O3_FORCE_INLINE uint64_t Thread_AtomicLoad64Relaxed(const Thread_Atomic64_t* object) {
	// Do a volatile load so that compiler doesn't duplicate loads, which makes
    // them nonatomic. (Happened in testing.)
    return ((volatile Thread_Atomic64_t*) object)->nonatomic;
}

AL2O3_FORCE_INLINE void Thread_AtomicStore64Relaxed(Thread_Atomic64_t* object, uint64_t value) {
	((volatile Thread_Atomic64_t*) object)->nonatomic = value;
	}

AL2O3_FORCE_INLINE uint64_t Thread_AtomicCompareExchange64Relaxed(Thread_Atomic64_t* object, uint64_t expected, uint64_t desired) {
	// Or make 2 versions of function? (Only if there's an advantage for GCC.)
	return _InterlockedCompareExchange64(object, desired, expected);
}

AL2O3_FORCE_INLINE intptr_t Thread_AtomicCompareExchangeWeak64Relaxed(Thread_Atomic64_t* object, uint64_t* expected, uint64_t desired) {
	uint64_t e = *expected;
	uint64_t previous = _InterlockedCompareExchange64(object, desired, e);
	intptr_t matched = (previous == e);
	if (!matched)
		*expected = previous;
	return matched;
}

AL2O3_FORCE_INLINE uint64_t Thread_AtomicExchange64Relaxed(Thread_Atomic64_t* object, uint64_t desired) {
	return _InterlockedExchange64(object, desired);
}

AL2O3_FORCE_INLINE uint64_t Thread_AtomicFetchAdd64Relaxed(Thread_Atomic64_t* object, int64_t operand) {
	return _InterlockedExchangeAdd64(object, operand);
}

AL2O3_FORCE_INLINE uint64_t Thread_AtomicFetchAnd64Relaxed(Thread_Atomic64_t* object, uint64_t operand) {
	return _InterlockedAnd64(object, operand);
}

AL2O3_FORCE_INLINE uint64_t Thread_AtomicFetchOr64Relaxed(Thread_Atomic64_t* object, uint64_t operand) {
	return _InterlockedOr64(object, operand);
}
