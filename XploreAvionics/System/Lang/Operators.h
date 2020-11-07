/*
 * CPP.h
 *
 *  Created on: 7 Nov 2020
 *      Author: arion
 */

#ifndef LANG_OPERATORS_H_
#define LANG_OPERATORS_H_

#include "cmsis_os.h"
#include <stdint.h>

void * operator new(size_t size) {
    return pvPortMalloc(size);
}

void * operator new[](size_t size) {
    return pvPortMalloc(size);
}

void operator delete(void* ptr) {
    vPortFree(ptr);
}

void operator delete[](void* ptr) {
    vPortFree(ptr);
}

#endif /* LANG_OPERATORS_H_ */
