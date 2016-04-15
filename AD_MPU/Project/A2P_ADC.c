#include<stm32f4xx.h>
#include<stdio.h>
#include "A2P_ADC.h"

/*定义ADC3的数据寄存器地址，DMA功能要用到外设的数据地址
 *ADC3的数据地址为外设基地址+偏移地址，基地址在RM0090 Reference
 *manual（参考手册）的地址映射表里，为0x40012200，ADC_DR
 *偏移地址为0x4C，故实际地址为0x40012200+0x4C = 0x4001224C */
#define ADC1_DR_Addr   ((uint32_t)0x4001204C)

__IO uint16_t ADCoverVaule;


/*************************************************
Function:    void ADC_Config(void)      
Description: ADC配置函数             
Input: 无                              
Output:无                              
Return:无                              
*************************************************/
void A2P_ADC_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		ADC_InitTypeDef ADC_InitStructure;
		ADC_CommonInitTypeDef ADC_CommonInitStructure;
	
		/* 使能GPIOC\GPIOF\GPIOG时钟*/
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		GPIO_StructInit(&GPIO_InitStructure);


		/* 初始化GPIOA的Pin_6为模拟量输入 */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);  
			
		
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //开ADC时钟
		ADC_DeInit();
		ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;  //精度为12位           
		ADC_InitStructure.ADC_ScanConvMode = DISABLE;   //扫描转换模式失能,单通道不用
		ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;  //连续转换使能
		ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; //不用外部触发，软件触发转换
		ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //数据右对齐，低字节对齐
		ADC_InitStructure.ADC_NbrOfConversion = 1;    //规定了顺序进行规则转换的ADC通道的数目
		ADC_Init(ADC1, &ADC_InitStructure);      

		ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;    //独立模式
		ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4; //分频为4，f(ADC3)=21M
		ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //失能DMA_MODE
		ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两次采样间隔5个周期
		ADC_CommonInit(&ADC_CommonInitStructure);

		ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 1, ADC_SampleTime_15Cycles); //规则通道配置，1表示规则组采样顺序
		//ADC_ITConfig(ADC3, ADC_IT_EOC, ENABLE); //使能ADC转换结束中断
		ADC_Cmd(ADC1, ENABLE);  //使能ADC3

		/*********************ADC看门狗配置***************************/
//		ADC_AnalogWatchdogCmd(ADC1, ADC_AnalogWatchdog_SingleRegEnable);
//		ADC_AnalogWatchdogThresholdsConfig(ADC1, 0x0E8B, 0x0555);  //阈值设置。高：3V 低：1V
//		ADC_AnalogWatchdogSingleChannelConfig(ADC1, ADC_Channel_6);
//		ADC_ITConfig(ADC1, ADC_IT_AWD, ENABLE);

		ADC_DMACmd(ADC1, ENABLE);   //使能ADC1的DMA  
		ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE); //单通道模式下上次转换完成后DMA请求允许，也就是持续DMA

		/* DMA中断配置 */
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);        //嵌套优先级分组为 1
		NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;   //嵌套通道为DMA2_Stream0_IRQn
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //抢占优先级为 1
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;    //响应优先级为 0
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;     //通道中断使能
		NVIC_Init(&NVIC_InitStructure);

		/* ADC中断配置 */
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);        //嵌套优先级分组为 1
		NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;           //嵌套通道为ADC_IRQn
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //抢占优先级为 1
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;    //响应优先级为 2
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;     //通道中断使能
		NVIC_Init(&NVIC_InitStructure);
}


/*************************************************
Function:    void DMA_Config(void)  
Description: DMA配置函数                  
Input: 延时的时间                      
Output:无                              
Return:无                              
*************************************************/
void A2P_ADCDMA_Init(void)
{
		DMA_InitTypeDef DMA_InitStructure;
		/*首先开DMA2时钟，由407参考手册-RM0090-Reference manual
		 *165页可知，UASRT6与DMA2映射，而且DMA2挂载在AHB1时钟总线上*/
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

		DMA_DeInit(DMA2_Stream0);
		DMA_StructInit( &DMA_InitStructure);
		DMA_InitStructure.DMA_Channel = DMA_Channel_0;           //选择Channel_0
		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_Addr; //数据传输的外设首地址，详解见上
		DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADCoverVaule;  //自己定义待发送数组的首地址，要强制转换为32位
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;    //数据传输方向选择为外设到内存

		DMA_InitStructure.DMA_BufferSize = 1;                      //传输数据大小为1，单位由以下确定，大小要配合定义的数组类型和外设数据类型
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //外设地址寄存器自动增加禁止，因为这里只用到了DR数据寄存器
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;    //内存地址自增不允许

		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //外设的数据大小，因为ADC6_DR数据寄存器为16为，故选HalfWord

		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;  //这里也选Byte
		DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;       //DMA传输模式为Circular,将会循环传输
		DMA_InitStructure.DMA_Priority = DMA_Priority_High; //优先级为High

		DMA_Init(DMA2_Stream0, &DMA_InitStructure);
		DMA_Cmd(DMA2_Stream0, ENABLE);      //使能DMA2_Stream0通道
		/* DMA中断开 */
		//DMA_ITConfig(DMA2_Stream6, DMA_IT_TC, ENABLE);
		DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE);
}

