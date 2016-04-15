#include "RTC.h"

unsigned char year_r; //读年
unsigned char month_r; //读月
unsigned char date_r; //读日
unsigned char xq_r; //读星期
unsigned char hours_r; //读时
unsigned char min_r; //读分
unsigned char sec_r; //读秒
unsigned char year_w; //写年
unsigned char month_w; //写月
unsigned char date_w; //写日
unsigned char xq_w; //写星期
unsigned char hours_w; //写时
unsigned char min_w; //写分
unsigned char sec_w; //写秒


/******************************************************************************
*
* 函数名: RTC_Config
* 功能: RTC 初始化
* 输入: None
* 输出: None
* 返回: None
******************************************************************************/
void RTC_Config(void)
{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE); //打开PWR 时钟
		PWR_BackupAccessCmd(ENABLE); //让路给RTC
		RCC_BackupResetCmd(ENABLE); //重新设定RTC
		RCC_BackupResetCmd(DISABLE); //重新设定RTC
		RCC_LSEConfig(RCC_LSE_ON); //打开时钟
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET) //等待时钟稳定
		{
		}
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE); //选择时钟来源
		RCC_RTCCLKCmd(ENABLE); //使能RTC 时钟
		RTC_WaitForSynchro(); //等待RTC 寄存器和APB 时钟同步
}

/******************************************************************************
*
* 函数名: RTC_TimeStampRead(void)
* 功能: 读时间日期
* 输入: None
* 输出: None
* 返回: None
******************************************************************************/
RTC_TimeStamp RTC_TimeStampRead(void)
{
		RTC_TimeTypeDef RTC_TimeStructure;
		RTC_DateTypeDef RTC_DateStructure;
		RTC_TimeStamp CurrentMoment;
		RTC_GetTime(RTC_Format_BCD, &RTC_TimeStructure); //读出时间
		RTC_GetDate(RTC_Format_BCD, &RTC_DateStructure); //读出日期
		CurrentMoment.hour = RTC_TimeStructure.RTC_Hours; //读出时送相应单元
		CurrentMoment.min = RTC_TimeStructure.RTC_Minutes; //读出分送相应单元
		CurrentMoment.sec = RTC_TimeStructure.RTC_Seconds; //读出秒送相应单元
		CurrentMoment.year = RTC_DateStructure.RTC_Year; //读出年送相应单元
		CurrentMoment.month = RTC_DateStructure.RTC_Month; //读出月送相应单元
		CurrentMoment.day = RTC_DateStructure.RTC_Date; //读出日送相应单元
		CurrentMoment.week = RTC_DateStructure.RTC_WeekDay; //读出星期送相应单元
		return CurrentMoment;
}

/******************************************************************************
*
* 函数名: RTC_TimeStampWrite
* 功能: 写时间日期
* 输入: None
* 输出: None
* 返回: None
******************************************************************************/
void RTC_TimeStampWrite(RTC_TimeStamp SetMoment)
{
		RTC_TimeTypeDef RTC_TimeStructure;
		RTC_DateTypeDef RTC_DateStructure;
		RTC_InitTypeDef RTC_InitStructure;
		RTC_TimeStructure.RTC_Hours = SetMoment.hour; //设置小时
		RTC_TimeStructure.RTC_Minutes = SetMoment.min; //设置分
		RTC_TimeStructure.RTC_Seconds = SetMoment.sec; //设置秒
		RTC_DateStructure.RTC_Year = SetMoment.year; //设置年
		RTC_DateStructure.RTC_Month = SetMoment.month; //设置月
		RTC_DateStructure.RTC_Date = SetMoment.day; //设置日
		RTC_DateStructure.RTC_WeekDay = SetMoment.week; //设置星期
		RTC_InitStructure.RTC_AsynchPrediv = 0x7F; //配置日历结构
		RTC_InitStructure.RTC_SynchPrediv = 0XFF;
		RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24; // 24 小时制
		RTC_Init(&RTC_InitStructure);
		RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure); //写入时间
		RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure); //写入日期
}

/******************************************************************************
*
* 函数名: RTC_TimeStampData
* 功能: 时间日期初值
* 输入: None
* 输出: None
* 返回: None
******************************************************************************/
void RTC_TimeStampData(RTC_TimeStamp * SetMoment)
{
		SetMoment->sec = 0x00;//秒初值，00 秒
		SetMoment->min = 0x00; //分初值，59 分
		SetMoment->hour = 0x00; //小时初值,23 时
		SetMoment->day = 0x06; //日初值,23 日
		SetMoment->month = 0x09; //月初值,8 月
		SetMoment->year = 0x15; //年初值,12 年
		SetMoment->week = RTC_Weekday_Sunday; //星期初值,星期4
}
