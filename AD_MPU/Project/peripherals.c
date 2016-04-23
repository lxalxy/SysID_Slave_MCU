#include "peripherals.h"

void Peripherals_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
    //����LED��IO��  
    //��ʼ��ʱ��  
//    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);  
//    //�ܽ�ģʽ:�����  
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;     
//    //����:����ģʽ  
//    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;    
//    //������������:��ʹ��  
//    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;      
//    //IO���ٶ�  
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  
//    //�ܽ�ָ��  
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;  
//    //��ʼ��  
//    GPIO_Init(GPIOD, &GPIO_InitStructure);  
      
    //���ð�����IO��  
    //��ʼ��ʱ��  
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  
    //�ܽ�ģʽ:�����  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;      
    //����:����ģʽ  
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;    
    //������������:��ʹ��  
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;      
    //IO���ٶ�  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  
    //�ܽ�ָ��  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;  
    //��ʼ��  
    GPIO_Init(GPIOB, &GPIO_InitStructure); 
		
		 //���ð�����IO��  
    //��ʼ��ʱ��  
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);  
    //�ܽ�ģʽ:�����  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;      
    //����:����ģʽ  
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;    
    //������������:��ʹ��  
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;      
    //IO���ٶ�  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  
    //�ܽ�ָ��  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;  
    //��ʼ��  
    GPIO_Init(GPIOE, &GPIO_InitStructure); 
}


void Timer3_Init(u16 arr,u16 psc)
{
		TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE); //��ʹ��TIM3ʱ��
		TIM_TimeBaseInitStructure.TIM_Period = arr; //�Զ���װ��ֵ
		TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
		TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
		TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
		TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);// �ڳ�ʼ����ʱ��TIM3
		TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //������ʱ��3�����ж�

		NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //��ʱ��3�ж�
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
		NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01; //��Ӧ���ȼ�3
		NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
		NVIC_Init(&NVIC_InitStructure);// �ܳ�ʼ��NVIC

		TIM_Cmd(TIM3,ENABLE); //��ʹ�ܶ�ʱ��3
}

void Timer4_Init(u16 arr,u16 psc)
{
		TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE); //��ʹ��TIM3ʱ��
		TIM_TimeBaseInitStructure.TIM_Period = arr; //�Զ���װ��ֵ
		TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
		TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
		TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
		TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);// �ڳ�ʼ����ʱ��TIM3
		TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); //������ʱ��3�����ж�

		NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn; //��ʱ��3�ж�
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
		NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01; //��Ӧ���ȼ�3
		NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
		NVIC_Init(&NVIC_InitStructure);// �ܳ�ʼ��NVIC

		TIM_Cmd(TIM4,ENABLE); //��ʹ�ܶ�ʱ��3
}
