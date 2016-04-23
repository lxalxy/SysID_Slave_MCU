#include "peripherals.h"

void Peripherals_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
    //设置LED的IO口  
    //初始化时钟  
//    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);  
//    //管脚模式:输出口  
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;     
//    //类型:推挽模式  
//    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;    
//    //上拉下拉设置:不使能  
//    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;      
//    //IO口速度  
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  
//    //管脚指定  
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;  
//    //初始化  
//    GPIO_Init(GPIOD, &GPIO_InitStructure);  
      
    //设置按键的IO口  
    //初始化时钟  
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  
    //管脚模式:输出口  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;      
    //类型:推挽模式  
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;    
    //上拉下拉设置:不使能  
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;      
    //IO口速度  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  
    //管脚指定  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;  
    //初始化  
    GPIO_Init(GPIOB, &GPIO_InitStructure); 
		
		 //设置按键的IO口  
    //初始化时钟  
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);  
    //管脚模式:输出口  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;      
    //类型:推挽模式  
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;    
    //上拉下拉设置:不使能  
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;      
    //IO口速度  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  
    //管脚指定  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;  
    //初始化  
    GPIO_Init(GPIOE, &GPIO_InitStructure); 
}


void Timer3_Init(u16 arr,u16 psc)
{
		TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE); //①使能TIM3时钟
		TIM_TimeBaseInitStructure.TIM_Period = arr; //自动重装载值
		TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
		TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
		TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
		TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);// ②初始化定时器TIM3
		TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //③允许定时器3更新中断

		NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //定时器3中断
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
		NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01; //响应优先级3
		NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
		NVIC_Init(&NVIC_InitStructure);// ④初始化NVIC

		TIM_Cmd(TIM3,ENABLE); //⑤使能定时器3
}

void Timer4_Init(u16 arr,u16 psc)
{
		TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE); //①使能TIM3时钟
		TIM_TimeBaseInitStructure.TIM_Period = arr; //自动重装载值
		TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
		TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
		TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
		TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);// ②初始化定时器TIM3
		TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); //③允许定时器3更新中断

		NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn; //定时器3中断
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
		NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01; //响应优先级3
		NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
		NVIC_Init(&NVIC_InitStructure);// ④初始化NVIC

		TIM_Cmd(TIM4,ENABLE); //⑤使能定时器3
}
