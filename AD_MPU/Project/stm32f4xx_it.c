/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Template/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    13-April-2012
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "stm324xg_eval_sdio_sd.h"
#include "main.h"
#include "delay.h"
#include "Mti.h"
#include "ff.h"

extern u8 current_state;
//extern struct Mti_Data RecievedData; 
extern __IO uint16_t ADCoverVaule;
//extern FIL *file;	  		//文件1
//extern unsigned int res;
//extern unsigned int bw;

u16 TimeCounter_1ms = 0;
u8  ReKeyFlag = 0; //重复按键标志
u8 KeyFlag=0;
u8 UsartMessage=0;
unsigned char status;      //0表示未接收数据,1表示读取到帧头1,2表示读取到帧头2,3表示读取到帧头3,4表示读取到长度,5表示数据读取完毕
unsigned char ccnt;        //计数
unsigned char packageflag; //是否接收到一个完整的数据包标志
unsigned char rxbuf[20];   //接收数据的缓冲区
//char buf[50];
//char isFirstOpenFile = 1;

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**名称：USART1中断服务程序
  */
void USART1_IRQHandler(void)
{
		if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
		{
				UsartMessage = USART_ReceiveData(USART1);
        while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
		}     
}


/**名称：DMA中断服务程序
  *作用：ADC3_DMA数据完全完成后产生中断
  */
void DMA2_Stream0_IRQHandler(void)
{
    if (DMA_GetITStatus(DMA2_Stream0, DMA_IT_TCIF0) != RESET)
		{
			DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TCIF0);
			/*添加用户代码*/
		}
}

/**名称：ADC看门狗中断服务程序
  *作用：ADC输入超过界限产生中断
  */
void ADC_IRQHandler(void)
{
		if (ADC_GetITStatus(ADC3, ADC_IT_AWD) == SET)
		{
				ADC_ClearITPendingBit(ADC3, ADC_IT_AWD);
				ADC_Cmd(ADC3, DISABLE);
				printf("Input is out of threrd!!");
		}
}


//void TIM4_IRQHandler(void)
//{
//		if(TIM_GetITStatus(TIM4, TIM_IT_Update)==SET) //溢出中断
//		{
//				if(isFirstOpenFile)
//				{
//						sprintf(buf, "TEST.TXT");
//						f_open(file, buf, FA_WRITE | FA_CREATE_ALWAYS);
//						isFirstOpenFile = 0;
//				}
//				GPIO_ToggleBits(GPIOB, GPIO_Pin_12);
//				//printf("%f %d\n", RecievedData.DATA[2].f, ADCoverVaule);
//				sprintf(buf, "%f %d\n", RecievedData.DATA[2].f, ADCoverVaule); //ADCoverVaule*3300/4096
//				res=f_write(file, buf, sizeof(buf), &bw);
//		}
//		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //清除中断标志位
//}


void TIM3_IRQHandler(void)
{
		if(TIM_GetITStatus(TIM3, TIM_IT_Update)==SET) //溢出中断
		{
				//printf("I'm in IRQ!\r\n");
			
				if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_0) == 0)
				{         
						//printf("I'm processing the key!\r\n");
						TimeCounter_1ms++;                         //有键按下，计数kcount加1
						//printf("The time that hold on the key is %d ms.\r\n", TimeCounter_1ms);
						if(TimeCounter_1ms >= 10)                  //按键已按下10ms
						{
								if(ReKeyFlag == 0)                //判断有没有重按键，1为有，0为没有
								{
										KeyFlag = 1;                     //设置按键标志
										//current_state = 1;
										TimeCounter_1ms = 0;        
										//ReKeyFlag = 1;                     //设置重按键标志
								}
								else
										TimeCounter_1ms = 0;
						}      
						else
								KeyFlag = 0;
						
						//printf("The key flag is %d.\r\n", KeyFlag);
				}
				else                                            //无按键按下
				{
						TimeCounter_1ms = 0;                       //清零kcount
						KeyFlag = 0;                            //清除按键标志
						ReKeyFlag = 0;                          //清除重按键标志
				}  
			//		GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
		 }				
		 TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //清除中断标志位
		// printf("I'm out IRQ!\r\n");
}


/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
   DUG_PRINTF("\n\r NMI_Handler ");
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  DUG_PRINTF("\n\r HardFault_Handler ");
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  DUG_PRINTF("\n\r MemManage_Handler ");
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  DUG_PRINTF("\n\r BusFault_Handler ");
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  DUG_PRINTF("\n\r UsageFault_Handler ");
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  //TimingDelay_Decrement();
}


void SDIO_IRQHandler(void)
{
	printf("SD_ProcessIRQSrc!\n");
	SD_ProcessIRQSrc();
}
void SD_SDIO_DMA_IRQHANDLER(void)
{
	printf("SD_ProcessDMAIRQ!\n");
	SD_ProcessDMAIRQ();
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
