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
	uint8_t previous;
	asm volatile("lock; cmpxchgb %2, %1" : "=a"(previous), "+m"(object->nonatomic) : "q"(desired), "0"(expected));
	return previous;
}

AL2O3_FORCE_INLINE intptr_t Thread_AtomicCompareExchangeWeak8Relaxed(Thread_Atomic8_t* object, uint8_t* expected, uint8_t desired) {
	uint8_t e = *expected;
	uint8_t previous;
	intptr_t matched;
	asm volatile("lock; cmpxchgb %2, %1" : "=a"(previous), "+m"(object->nonatomic) : "q"(desired), "0"(e));
	matched = (previous == e);
	if (!matched)
		*expected = previous;
	return matched;
}

AL2O3_FORCE_INLINE uint8_t Thread_AtomicExchange8Relaxed(Thread_Atomic8_t* object, uint8_t desired) {
	uint8_t previous;
	asm volatile("xchgb %0, %1" : "=r"(previous), "+m"(object->nonatomic) : "0"(desired));
	return previous;
}

AL2O3_FORCE_INLINE uint8_t Thread_AtomicFetchAdd8Relaxed(Thread_Atomic8_t* object, int8_t operand) {
	uint8_t previous;
	asm volatile("lock; xaddb %0, %1" : "=r"(previous), "+m"(object->nonatomic) : "0"(operand));
	return previous;
}

AL2O3_FORCE_INLINE uint8_t Thread_AtomicFetchAnd8Relaxed(Thread_Atomic8_t* object, uint8_t operand) {
	uint8_t previous;
	uint8_t temp;
	asm volatile("1:     movb    %1, %0\n"
							 "       movb    %0, %2\n"
							 "       andb    %3, %2\n"
							 "       lock; cmpxchgb %2, %1\n"
							 "       jne     1b"
	: "=&a"(previous), "+m"(object->nonatomic), "=&r"(temp)
	: "r"(operand));
	return previous;
}

AL2O3_FORCE_INLINE uint8_t Thread_AtomicFetchOr8Relaxed(Thread_Atomic8_t* object, uint8_t operand) {
	uint8_t previous;
	uint8_t temp;
	asm volatile("1:     movb    %1, %0\n"
							 "       movb    %0, %2\n"
							 "       orb     %3, %2\n"
							 "       lock; cmpxchgb %2, %1\n"
							 "       jne     1b"
	: "=&a"(previous), "+m"(object->nonatomic), "=&r"(temp)
	: "r"(operand));
	return previous;
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
	uint16_t previous;
	asm volatile("lock; cmpxchgw %2, %1" : "=a"(previous), "+m"(object->nonatomic) : "q"(desired), "0"(expected));
	return previous;
}

AL2O3_FORCE_INLINE intptr_t Thread_AtomicCompareExchangeWeak16Relaxed(Thread_Atomic16_t* object, uint16_t* expected, uint16_t desired) {
	uint16_t e = *expected;
	uint16_t previous;
	intptr_t matched;
	asm volatile("lock; cmpxchgw %2, %1" : "=a"(previous), "+m"(object->nonatomic) : "q"(desired), "0"(e));
	matched = (previous == e);
	if (!matched)
		*expected = previous;
	return matched;
}

AL2O3_FORCE_INLINE uint16_t Thread_AtomicExchange16Relaxed(Thread_Atomic16_t* object, uint16_t desired) {
	uint16_t previous;
	asm volatile("xchgw %0, %1" : "=r"(previous), "+m"(object->nonatomic) : "0"(desired));
	return previous;
}

AL2O3_FORCE_INLINE uint16_t Thread_AtomicFetchAdd16Relaxed(Thread_Atomic16_t* object, int16_t operand) {
	uint16_t previous;
	asm volatile("lock; xaddw %0, %1" : "=r"(previous), "+m"(object->nonatomic) : "0"(operand));
	return previous;
}

AL2O3_FORCE_INLINE uint16_t Thread_AtomicFetchAnd16Relaxed(Thread_Atomic16_t* object, uint16_t operand) {
	uint16_t previous;
	uint16_t temp;
	asm volatile("1:     movw    %1, %0\n"
							 "       movw    %0, %2\n"
							 "       andw    %3, %2\n"
							 "       lock; cmpxchgw %2, %1\n"
							 "       jne     1b"
			: "=&a"(previous), "+m"(object->nonatomic), "=&r"(temp)
			: "r"(operand));
	return previous;
}

AL2O3_FORCE_INLINE uint16_t Thread_AtomicFetchOr16Relaxed(Thread_Atomic16_t* object, uint16_t operand) {
	uint16_t previous;
	uint16_t temp;
	asm volatile("1:     movw    %1, %0\n"
							 "       movw    %0, %2\n"
							 "       orw     %3, %2\n"
							 "       lock; cmpxchgw %2, %1\n"
							 "       jne     1b"
				: "=&a"(previous), "+m"(object->nonatomic), "=&r"(temp)
				: "r"(operand));
	return previous;
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
	// CMPXCHG is written cmpxchgl because GCC (and Clang) uses AT&T assembler
	// syntax.
	// Also due to AT&T syntax, the operands are swapped: %1 is the destination.
	// (This is the opposite of how Intel syntax lists the operands, where the
	// destination comes first.)
	// "=a"(previous) means the asm block outputs EAX to previous, because CMPXCHG
	// puts the old value in EAX.
	// "+m"(object->nonatomic) is the memory address that is read/written. This
	// becomes %1.
	// "q"(desired) puts desired into any of EBX, ECX or EDX before the block.
	// This becomes %2.
	// "0"(expected) puts expected in the same register as "=a"(previous), which
	// is EAX, before the block.
	// Not putting "memory" in the clobber list because the operation is relaxed.
	// It's OK for the compiler
	// to reorder this atomic followed by a load, for example. If the programmer
	// wants to enforce ordering,
	// they will use an explicit fence.
	// http://www.ibiblio.org/gferg/ldp/GCC-Inline-Assembly-HOWTO.html
	// http://gcc.gnu.org/onlinedocs/gcc/Simple-Constraints.html#Simple-Constraints
	// http://gcc.gnu.org/onlinedocs/gcc/Modifiers.html#Modifiers
	// http://gcc.gnu.org/onlinedocs/gcc/Machine-Constraints.html#Machine-Constraints
	// http://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html
	// http://download.intel.com/products/processor/manual/325383.pdf
	uint32_t previous;
	asm volatile("lock; cmpxchgl %2, %1" : "=a"(previous), "+m"(object->nonatomic) : "q"(desired), "0"(expected));
	return previous;
}

AL2O3_FORCE_INLINE intptr_t Thread_AtomicCompareExchangeWeak32Relaxed(Thread_Atomic32_t* object, uint32_t* expected, uint32_t desired) {
	uint32_t e = *expected;
	uint32_t previous;
	intptr_t matched;
	asm volatile("lock; cmpxchgl %2, %1" : "=a"(previous), "+m"(object->nonatomic) : "q"(desired), "0"(e));
	matched = (previous == e);
	if (!matched)
	*expected = previous;
	return matched;
}

AL2O3_FORCE_INLINE uint32_t Thread_AtomicExchange32Relaxed(Thread_Atomic32_t* object, uint32_t desired) {
	// No lock prefix is necessary for XCHG.
	// See turf_fetchAdd32Relaxed for explanation of constraints.
	uint32_t previous;
	asm volatile("xchgl %0, %1" : "=r"(previous), "+m"(object->nonatomic) : "0"(desired));
	return previous;
}

AL2O3_FORCE_INLINE uint32_t Thread_AtomicFetchAdd32Relaxed(Thread_Atomic32_t* object, int32_t operand) {
	// "=r"(previous) chooses any general register, makes that %0, and outputs
	// this register to previous after the
	// block.
	// "+m"(object->nonatomic) is the memory address that is read/written. This
	// becomes %1.
	// "0"(operand) puts operand into the same register as %0 before the block.
	// volatile is required. Otherwise, if the return value (previous) is unused,
	// the asm block
	// may be deleted. ("+m" is apparently not enough hint to the compiler that
	// the asm
	// block has side effects on memory.)
	uint32_t previous;
	asm volatile("lock; xaddl %0, %1" : "=r"(previous), "+m"(object->nonatomic) : "0"(operand));
	return previous;
}

AL2O3_FORCE_INLINE uint32_t Thread_AtomicFetchAnd32Relaxed(Thread_Atomic32_t* object, uint32_t operand) {
	// The & in "=&a"(previous) makes eax an earlyclobber operand.
	// If we don't specify &, the compiler may assign eax to input operand %3 as
	// well.
	uint32_t previous;
	uint32_t temp;
	asm volatile("1:     movl    %1, %0\n"
							 "       movl    %0, %2\n"
							 "       andl    %3, %2\n"
							 "       lock; cmpxchgl %2, %1\n"
							 "       jne     1b"
	: "=&a"(previous), "+m"(object->nonatomic), "=&r"(temp)
	: "r"(operand));
	return previous;
}

AL2O3_FORCE_INLINE uint32_t Thread_AtomicFetchOr32Relaxed(Thread_Atomic32_t* object, uint32_t operand) {
	uint32_t previous;
	uint32_t temp;
	asm volatile("1:     movl    %1, %0\n"
							 "       movl    %0, %2\n"
							 "       orl     %3, %2\n"
							 "       lock; cmpxchgl %2, %1\n"
							 "       jne     1b"
	: "=&a"(previous), "+m"(object->nonatomic), "=&r"(temp)
	: "r"(operand));
	return previous;
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
    // On x64, we can work with 64-bit values directly.
    // It's basically the same as the 32-bit versions except for the q suffix on
    // opcodes.
    uint64_t previous;
    asm volatile("lock; cmpxchgq %2, %1" : "=a"(previous), "+m"(object->nonatomic) : "q"(desired), "0"(expected));
    return previous;
}

AL2O3_FORCE_INLINE intptr_t Thread_AtomicCompareExchangeWeak64Relaxed(Thread_Atomic64_t* object, uint64_t* expected, uint64_t desired) {
    uint64_t e = *expected;
    uint64_t previous;
		intptr_t matched;
    asm volatile("lock; cmpxchgq %2, %1" : "=a"(previous), "+m"(object->nonatomic) : "q"(desired), "0"(e));
    matched = (previous == e);
    if (!matched)
        *expected = previous;
    return matched;
}

AL2O3_FORCE_INLINE uint64_t Thread_AtomicExchange64Relaxed(Thread_Atomic64_t* object, uint64_t desired) {
    uint64_t previous;
    asm volatile("xchgq %0, %1" : "=r"(previous), "+m"(object->nonatomic) : "0"(desired));
    return previous;
}

AL2O3_FORCE_INLINE uint64_t Thread_AtomicFetchAdd64Relaxed(Thread_Atomic64_t* object, int64_t operand) {
    uint64_t previous;
    asm volatile("lock; xaddq %0, %1" : "=r"(previous), "+m"(object->nonatomic) : "0"(operand));
    return previous;
}

AL2O3_FORCE_INLINE uint64_t Thread_AtomicFetchAnd64Relaxed(Thread_Atomic64_t* object, uint64_t operand) {
    uint64_t previous;
    uint64_t temp;
    asm volatile("1:     movq    %1, %0\n"
                 "       movq    %0, %2\n"
                 "       andq    %3, %2\n"
                 "       lock; cmpxchgq %2, %1\n"
                 "       jne     1b"
                 : "=&a"(previous), "+m"(object->nonatomic), "=&r"(temp)
                 : "r"(operand));
    return previous;
}

AL2O3_FORCE_INLINE uint64_t Thread_AtomicFetchOr64Relaxed(Thread_Atomic64_t* object, uint64_t operand) {
    uint64_t previous;
    uint64_t temp;
    asm volatile("1:     movq    %1, %0\n"
                 "       movq    %0, %2\n"
                 "       orq     %3, %2\n"
                 "       lock; cmpxchgq %2, %1\n"
                 "       jne     1b"
                 : "=&a"(previous), "+m"(object->nonatomic), "=&r"(temp)
                 : "r"(operand));
    return previous;
}