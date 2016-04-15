#include "stm32f4xx.h"

typedef struct RTC_TimeStamp{
		uint8_t year; //��
		uint8_t month; //��
		uint8_t day; //��
		uint8_t week; //����
		uint8_t hour; //ʱ
		uint8_t min; //��
		uint8_t sec; //��
}RTC_TimeStamp;

void RTC_Config(void); //��������
void RTC_TimeStampData(RTC_TimeStamp * SetMoment); //��������
RTC_TimeStamp RTC_TimeStampRead(void); //��������
void RTC_TimeStampWrite(RTC_TimeStamp SetMoment); //��������
