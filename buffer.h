/*
 * DESCRIPTION:	header file for circular buffer 
 *
 * LICENSE: LGPL
 *              
 * REVISION HISTORY:
 *
 * Num      Modified by            Date            Reason
 * ------   -----------------      -------------   -----------------
 * 100      Melvin                 2013-1-14       Created.
 *
 */
#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <stdlib.h>

typedef void* BufferHandle;

#define BUFFER_TRUE				 1
#define BUFFER_FALSE			 0

#define BUFFER_SUCCESS			 0
#define BUFFER_MALLOC_FAIL 		-1
#define BUFFER_IS_FULL	   		-2
#define BUFFER_IS_EMPTY	   		-3
#define BUFFER_INVALID_HANDLE 	-4

int32_t	bufferInit(BufferHandle* pBufferHandle, int32_t recordSize, int32_t recordSum);
void* 	bufferPeer(BufferHandle bufferHandle, int32_t index);
void	bufferFree(BufferHandle bufferHandle);
void 	bufferClear(BufferHandle bufferHandle);
int32_t	bufferRead(BufferHandle bufferHandle, void* pRecord);
int32_t	bufferWrite(BufferHandle bufferHandle, void* pRecord);
int32_t	bufferIsEmpty(BufferHandle bufferHandle);
int32_t	bufferIsFull(BufferHandle bufferHandle);
int32_t	bufferSize(BufferHandle bufferHandle);

#endif

