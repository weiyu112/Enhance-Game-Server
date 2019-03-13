





#include <string.h>
#include <stdio.h>

#include "NattyAbstractClass.h"

void *New(const void *_class, ...) {
	const AbstractClass *mclass = (const AbstractClass *)_class;
	void *p = calloc(1, mclass->size);
	memset(p, 0, mclass->size);
	
	assert(p);
	*(const AbstractClass**)p = mclass;
	
	if (mclass->ctor) {
		va_list params;
		va_start(params, _class);
		p = mclass->ctor(p, &params);
		va_end(params);
	}
	return p;
}


void Delete(void *_class) {
	const AbstractClass **mclass = (const AbstractClass **)_class;

	if (_class && (*mclass) && (*mclass)->dtor) {
		_class = (*mclass)->dtor(_class);
	}
	
	free(_class);
}



unsigned long cmpxchg(void *addr, unsigned long _old, unsigned long _new, int size) {
	unsigned long prev;
	volatile unsigned int *_ptr = (volatile unsigned int *)(addr);

	switch (size) {
		case BYTE_WIDTH: {
			__asm__ volatile (
		        "lock; cmpxchgb %b1, %2"
		        : "=a" (prev)
		        : "r" (_new), "m" (*_ptr), "0" (_old)
		        : "memory");
			break;
		}
		case WCHAR_WIDTH: {
			__asm__ volatile (
		        "lock; cmpxchgw %w1, %2"
		        : "=a" (prev)
		        : "r" (_new), "m" (*_ptr), "0" (_old)
		        : "memory");
			break;
		}
		case WORD_WIDTH: {
			__asm__ volatile (
		        "lock; cmpxchg %1, %2"
		        : "=a" (prev)
		        : "r" (_new), "m" (*_ptr), "0" (_old)
		        : "memory");
			break;
		}
	}

	return prev;
}


