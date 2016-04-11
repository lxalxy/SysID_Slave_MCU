#include "RTC.h"

unsigned char year_r; //����
unsigned char month_r; //����
unsigned char date_r; //����
unsigned char xq_r; //������
unsigned char hours_r; //��ʱ
unsigned char min_r; //����
unsigned char sec_r; //����
unsigned char year_w; //д��
unsigned char month_w; //д��
unsigned char date_w; //д��
unsigned char xq_w; //д����
unsigned char hours_w; //дʱ
unsigned char min_w; //д��
unsigned char sec_w; //д��


/******************************************************************************
*
* ������: RTC_Config
* ����: RTC ��ʼ��
* ����: None
* ���: None
* ����: None
******************************************************************************/
void RTC_Config(void)
{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE); //��PWR ʱ��
		PWR_BackupAccessCmd(ENABLE); //��·��RTC
		RCC_BackupResetCmd(ENABLE); //�����趨RTC
		RCC_BackupResetCmd(DISABLE); //�����趨RTC
		RCC_LSEConfig(RCC_LSE_ON); //��ʱ��
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET) //�ȴ�ʱ���ȶ�
		{
		}
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE); //ѡ��ʱ����Դ
		RCC_RTCCLKCmd(ENABLE); //ʹ��RTC ʱ��
		RTC_WaitForSynchro(); //�ȴ�RTC �Ĵ�����APB ʱ��ͬ��
}

/******************************************************************************
*
* ������: RTC_TimeStampRead(void)
* ����: ��ʱ������
* ����: None
* ���: None
* ����: None
******************************************************************************/
RTC_TimeStamp RTC_TimeStampRead(void)
{
		RTC_TimeTypeDef RTC_TimeStructure;
		RTC_DateTypeDef RTC_DateStructure;
		RTC_TimeStamp CurrentMoment;
		RTC_GetTime(RTC_Format_BCD, &RTC_TimeStructure); //����ʱ��
		RTC_GetDate(RTC_Format_BCD, &RTC_DateStructure); //��������
		CurrentMoment.hour = RTC_TimeStructure.RTC_Hours; //����ʱ����Ӧ��Ԫ
		CurrentMoment.min = RTC_TimeStructure.RTC_Minutes; //����������Ӧ��Ԫ
		CurrentMoment.sec = RTC_TimeStructure.RTC_Seconds; //����������Ӧ��Ԫ
		CurrentMoment.year = RTC_DateStructure.RTC_Year; //����������Ӧ��Ԫ
		CurrentMoment.month = RTC_DateStructure.RTC_Month; //����������Ӧ��Ԫ
		CurrentMoment.day = RTC_DateStructure.RTC_Date; //����������Ӧ��Ԫ
		CurrentMoment.week = RTC_DateStructure.RTC_WeekDay; //������������Ӧ��Ԫ
		return CurrentMoment;
}

/******************************************************************************
*
* ������: RTC_TimeStampWrite
* ����: дʱ������
* ����: None
* ���: None
* ����: None
******************************************************************************/
void RTC_TimeStampWrite(RTC_TimeStamp SetMoment)
{
		RTC_TimeTypeDef RTC_TimeStructure;
		RTC_DateTypeDef RTC_DateStructure;
		RTC_InitTypeDef RTC_InitStructure;
		RTC_TimeStructure.RTC_Hours = SetMoment.hour; //����Сʱ
		RTC_TimeStructure.RTC_Minutes = SetMoment.min; //���÷�
		RTC_TimeStructure.RTC_Seconds = SetMoment.sec; //������
		RTC_DateStructure.RTC_Year = SetMoment.year; //������
		RTC_DateStructure.RTC_Month = SetMoment.month; //������
		RTC_DateStructure.RTC_Date = SetMoment.day; //������
		RTC_DateStructure.RTC_WeekDay = SetMoment.week; //��������
		RTC_InitStructure.RTC_AsynchPrediv = 0x7F; //���������ṹ
		RTC_InitStructure.RTC_SynchPrediv = 0XFF;
		RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24; // 24 Сʱ��
		RTC_Init(&RTC_InitStructure);
		RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure); //д��ʱ��
		RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure); //д������
}

/******************************************************************************
*
* ������: RTC_TimeStampData
* ����: ʱ�����ڳ�ֵ
* ����: None
* ���: None
* ����: None
******************************************************************************/
void RTC_TimeStampData(RTC_TimeStamp * SetMoment)
{
		SetMoment->sec = 0x00;//���ֵ��00 ��
		SetMoment->min = 0x00; //�ֳ�ֵ��59 ��
		SetMoment->hour = 0x00; //Сʱ��ֵ,23 ʱ
		SetMoment->day = 0x06; //�ճ�ֵ,23 ��
		SetMoment->month = 0x09; //�³�ֵ,8 ��
		SetMoment->year = 0x15; //���ֵ,12 ��
		SetMoment->week = RTC_Weekday_Sunday; //���ڳ�ֵ,����4
}
