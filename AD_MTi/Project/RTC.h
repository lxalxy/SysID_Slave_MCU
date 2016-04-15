#include "stm32f4xx.h"

typedef struct RTC_TimeStamp{
		uint8_t year; //年
		uint8_t month; //月
		uint8_t day; //日
		uint8_t week; //星期
		uint8_t hour; //时
		uint8_t min; //分
		uint8_t sec; //秒
}RTC_TimeStamp;

void RTC_Config(void); //函数声明
void RTC_TimeStampData(RTC_TimeStamp * SetMoment); //函数声明
RTC_TimeStamp RTC_TimeStampRead(void); //函数声明
void RTC_TimeStampWrite(RTC_TimeStamp SetMoment); //函数声明
