#include "Mti.h"

void Mti_Init(struct Mti_Data* Mti_TempData)
{
		NVIC_InitTypeDef NVIC_InitStructure;
		
	
		Mti_TempData->CHECKSUM = 0;
		Mti_TempData->DATA[0].i = 0;
		Mti_TempData->DATA[1].i = 0;
		Mti_TempData->DATA[2].i = 0;
		Mti_TempData->LEN = 0;

		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);  //嵌套优先级分组为 
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; //嵌套通道为USART1_IRQn
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //抢占优先级为 
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;    //响应优先级为 
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;     //通道中断使能
		NVIC_Init(&NVIC_InitStructure);   
}
