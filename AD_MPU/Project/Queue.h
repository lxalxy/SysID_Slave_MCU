#ifndef _QUEUE_H_
#define _QUEUE_H_


#include "stdint.h"

#define ASSERT_PARAM(a)           if (a == NULL) return 0;

typedef struct {
 volatile uint32_t     bufSize;
 volatile uint8_t      *pStart;
 volatile uint8_t      *pEnd;
 volatile uint8_t      *pBuf;
}QUEUE8_TYPE;

uint32_t QUEUE_PacketCreate(QUEUE8_TYPE *pQ8, uint8_t *pBuf, uint32_t bufSize);
uint32_t QUEUE_PacketIn(QUEUE8_TYPE *pQ8, uint8_t *pData, uint32_t len);
uint32_t QUEUE_PacketOut(QUEUE8_TYPE *pQ8, uint8_t *pData, uint32_t dataLen);
uint32_t QUEUE_PacketSplit(QUEUE8_TYPE *pQ8, uint8_t startChar, uint8_t endChar, uint8_t *pData, uint32_t dataLen);
uint32_t QUEUE_PacketDoubleByteSplit(QUEUE8_TYPE *pQ8, uint8_t splitChar, uint8_t *pData, uint32_t dataLen);
uint32_t QUEUE_PacketCharSplit(QUEUE8_TYPE *pQ8, uint8_t splitChar, uint8_t *pData, uint32_t dataLen);
uint32_t QUEUE_PacketDoubleCharSplit(QUEUE8_TYPE *pQ8, uint8_t splitChar1, uint8_t splitChar2, uint8_t *pData, uint32_t dataLen);
 
#endif
