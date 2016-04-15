#include "Mti.h"

void Mti_Init(struct Mti_Data* Mti_TempData)
{
		NVIC_InitTypeDef NVIC_InitStructure;
		
	
		Mti_TempData->CHECKSUM = 0;
		Mti_TempData->DATA[0].i = 0;
		Mti_TempData->DATA[1].i = 0;
		Mti_TempData->DATA[2].i = 0;
		Mti_TempData->LEN = 0;

		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);  //Ƕ�����ȼ�����Ϊ 
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; //Ƕ��ͨ��ΪUSART1_IRQn
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //��ռ���ȼ�Ϊ 
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;    //��Ӧ���ȼ�Ϊ 
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;     //ͨ���ж�ʹ��
		NVIC_Init(&NVIC_InitStructure);   
}
