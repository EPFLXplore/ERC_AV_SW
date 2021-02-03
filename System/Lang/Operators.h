/*
 * CPP.h
 *
 *  Created on: 7 Nov 2020
 *      Author: AV Team 2020
 */

#ifndef LANG_OPERATORS_H_
#define LANG_OPERATORS_H_

#include <cstdint>
#include <cstddef>

void * operator new(size_t size);

void * operator new[](size_t size);

void operator delete(void* ptr);

void operator delete[](void* ptr);

#endif /* LANG_OPERATORS_H_ */
