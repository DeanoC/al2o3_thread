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

//-------------------------------------
//  Atomic types
//-------------------------------------
typedef struct { volatile uint8_t nonatomic; } Thread_Atomic8_t;
typedef struct { volatile uint16_t nonatomic; } Thread_Atomic16_t __attribute__((aligned(2)));
typedef struct { volatile uint32_t nonatomic; } Thread_Atomic32_t __attribute__((aligned(4)));
typedef struct { volatile uint64_t nonatomic; } Thread_Atomic64_t __attribute__((aligned(8)));
typedef struct { volatile uint128_t nonatomic; } Thread_Atomic128_t __attribute__((aligned(16)));

typedef struct { void* volatile nonatomic; } Thread_AtomicPtr_t __attribute__((aligned(8)));

//-------------------------------------
//  Fences
//-------------------------------------
#define Thread_AtomicSignalFenceConsume() (0)
#define Thread_AtomicSignalFenceAcquire() asm volatile("" ::: "memory")
#define Thread_AtomicSignalFenceRelease() asm volatile("" ::: "memory")
#define Thread_AtomicSignalFenceSeqCst() asm volatile("" ::: "memory")

#define Thread_AtomicThreadFenceConsume() (0)
#define Thread_AtomicThreadFenceAcquire() asm volatile("" ::: "memory")
#define Thread_AtomicThreadFenceRelease() asm volatile("" ::: "memory")
#define Thread_AtomicThreadFenceSeqCst() asm volatile("lock; orl $0, (%%rsp)" ::: "memory")

//----------------------------------------------
//  8-bit atomic operations
//----------------------------------------------
AL2O3_FORCE_INLINE uint8_t Thread_AtomicLoad8Relaxed(const Thread_Atomic8_t* object) {
	return object->nonatomic;
}

AL2O3_FORCE_INLINE void Thread_AtomicStore8Relaxed(Thread_Atomic8_t* object, uint8_t desired) {
	object->nonatomic = desired;
}

AL2O3_FORCE_INLINE uint8_t Thread_AtomicCompareExchange8Relaxed(Thread_Atomic8_t* object, uint8_t expected, uint8_t desired) {
	return __sync_val_compare_and_swap(&object->nonatomic, expected, desired);
}

AL2O3_FORCE_INLINE uint8_t Thread_AtomicExchange8Relaxed(Thread_Atomic8_t* object, uint8_t desired) {
	uint8_t previous;
	asm volatile("xchgb %0, %1" : "=r"(previous), "+m"(object->nonatomic) : "0"(desired));
	return previous;
}

AL2O3_FORCE_INLINE uint8_t Thread_AtomicFetchAdd8Relaxed(Thread_Atomic8_t* object, int8_t operand) {
	return __sync_fetch_and_add(&object->nonatomic, operand);
}

AL2O3_FORCE_INLINE uint8_t Thread_AtomicFetchAnd8Relaxed(Thread_Atomic8_t* object, uint8_t operand) {
	return __sync_fetch_and_and(&object->nonatomic, operand);
}

AL2O3_FORCE_INLINE uint8_t Thread_AtomicFetchOr8Relaxed(Thread_Atomic8_t* object, uint8_t operand) {
	return __sync_fetch_and_or(&object->nonatomic, operand);
}

//----------------------------------------------
//  16-bit atomic operations
//----------------------------------------------
AL2O3_FORCE_INLINE uint16_t Thread_AtomicLoad16Relaxed(const Thread_Atomic16_t* object) {
	return object->nonatomic;
}

AL2O3_FORCE_INLINE void Thread_AtomicStore16Relaxed(Thread_Atomic16_t* object, uint16_t desired) {
	object->nonatomic = desired;
}

AL2O3_FORCE_INLINE uint16_t Thread_AtomicCompareExchange16Relaxed(Thread_Atomic16_t* object, uint16_t expected, uint16_t desired) {
	return __sync_val_compare_and_swap(&object->nonatomic, expected, desired);
}

AL2O3_FORCE_INLINE uint16_t Thread_AtomicExchange16Relaxed(Thread_Atomic16_t* object, uint16_t desired) {
	uint16_t previous;
	asm volatile("xchgw %0, %1" : "=r"(previous), "+m"(object->nonatomic) : "0"(desired));
	return previous;
}

AL2O3_FORCE_INLINE uint16_t Thread_AtomicFetchAdd16Relaxed(Thread_Atomic16_t* object, int16_t operand) {
	return __sync_fetch_and_add(&object->nonatomic, operand);
}

AL2O3_FORCE_INLINE uint16_t Thread_AtomicFetchAnd16Relaxed(Thread_Atomic16_t* object, uint16_t operand) {
	return __sync_fetch_and_and(&object->nonatomic, operand);
}

AL2O3_FORCE_INLINE uint16_t Thread_AtomicFetchOr16Relaxed(Thread_Atomic16_t* object, uint16_t operand) {
	return __sync_fetch_and_or(&object->nonatomic, operand);
}

//----------------------------------------------
//  32-bit atomic operations
//----------------------------------------------
AL2O3_FORCE_INLINE uint32_t Thread_AtomicLoad32Relaxed(const Thread_Atomic32_t* object) {
	return object->nonatomic;
}

AL2O3_FORCE_INLINE void Thread_AtomicStore32Relaxed(Thread_Atomic32_t* object, uint32_t desired) {
	object->nonatomic = desired;
}

AL2O3_FORCE_INLINE uint32_t Thread_AtomicCompareExchange32Relaxed(Thread_Atomic32_t* object, uint32_t expected, uint32_t desired) {
	return __sync_val_compare_and_swap(&object->nonatomic, expected, desired);
}

AL2O3_FORCE_INLINE uint32_t Thread_AtomicExchange32Relaxed(Thread_Atomic32_t* object, uint32_t desired) {
	// No lock prefix is necessary for XCHG.
	// See turf_fetchAdd32Relaxed for explanation of constraints.
	uint32_t previous;
	asm volatile("xchgl %0, %1" : "=r"(previous), "+m"(object->nonatomic) : "0"(desired));
	return previous;
}

AL2O3_FORCE_INLINE uint32_t Thread_AtomicFetchAdd32Relaxed(Thread_Atomic32_t* object, int32_t operand) {
	return __sync_fetch_and_add(&object->nonatomic, operand);
}

AL2O3_FORCE_INLINE uint32_t Thread_AtomicFetchAnd32Relaxed(Thread_Atomic32_t* object, uint32_t operand) {
	return __sync_fetch_and_and(&object->nonatomic, operand);
}

AL2O3_FORCE_INLINE uint32_t Thread_AtomicFetchOr32Relaxed(Thread_Atomic32_t* object, uint32_t operand) {
	return __sync_fetch_and_or(&object->nonatomic, operand);
}

//------------------------------------------------------------------------
//  64-bit atomic operations on 64-bit processor (x64)
//------------------------------------------------------------------------

AL2O3_FORCE_INLINE uint64_t Thread_AtomicLoad64Relaxed(const Thread_Atomic64_t* object) {
    // On x64, aligned 64-bit loads are already atomic.
    return object->nonatomic;
}

AL2O3_FORCE_INLINE void Thread_AtomicStore64Relaxed(Thread_Atomic64_t* object, uint64_t desired) {
    // On x64, aligned 64-bit stores are already atomic.
    object->nonatomic = desired;
}

AL2O3_FORCE_INLINE uint64_t Thread_AtomicCompareExchange64Relaxed(Thread_Atomic64_t* object, uint64_t expected, uint64_t desired) {
	return __sync_val_compare_and_swap(&object->nonatomic, expected, desired);
}

AL2O3_FORCE_INLINE uint64_t Thread_AtomicExchange64Relaxed(Thread_Atomic64_t* object, uint64_t desired) {
    uint64_t previous;
    asm volatile("xchgq %0, %1" : "=r"(previous), "+m"(object->nonatomic) : "0"(desired));
    return previous;
}

AL2O3_FORCE_INLINE uint64_t Thread_AtomicFetchAdd64Relaxed(Thread_Atomic64_t* object, int64_t operand) {
	return __sync_fetch_and_add(&object->nonatomic, operand);
}

AL2O3_FORCE_INLINE uint64_t Thread_AtomicFetchAnd64Relaxed(Thread_Atomic64_t* object, uint64_t operand) {
	return __sync_fetch_and_and(&object->nonatomic, operand);
}

AL2O3_FORCE_INLINE uint64_t Thread_AtomicFetchOr64Relaxed(Thread_Atomic64_t* object, uint64_t operand) {
	return __sync_fetch_and_or(&object->nonatomic, operand);
}

// 128 bit atomics
AL2O3_FORCE_INLINE uint128_t Thread_AtomicLoad128Relaxed(Thread_Atomic128_t* object) {
	return (uint128_t)__sync_fetch_and_or((__int128*)&object->nonatomic, 0);
}

AL2O3_FORCE_INLINE void Thread_AtomicStore128Relaxed(Thread_Atomic128_t* object, uint128_t desired) {
	// x64 with cx16 can handle 128 bit atomics
	while(!__sync_bool_compare_and_swap((__int128*)&object->nonatomic, (__int128)Thread_AtomicLoad128Relaxed(object), (__int128)desired))
	{
		// try again
	}
}

AL2O3_FORCE_INLINE uint128_t Thread_AtomicCompareExchange128Relaxed(Thread_Atomic128_t* object, uint128_t expected, uint128_t desired) {
	return (uint128_t)__sync_val_compare_and_swap((__int128*)&object->nonatomic, (__int128)expected, (__int128)desired);
}
