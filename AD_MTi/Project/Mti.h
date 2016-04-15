#include "stm32f4xx.h"

union data{
		uint64_t i;
		float f;
};

struct Mti_Data{
//		uint8_t PREAMBLE;
//		uint8_t BID;
//		uint8_t MID;
		uint8_t LEN;
		union data DATA[3];
		uint8_t CHECKSUM;
};


void Mti_Init(struct Mti_Data*);
