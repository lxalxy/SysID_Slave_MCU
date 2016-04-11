/**
  ******************************************************************************
  * @file    Printf.c 
  * @author  MCD Application Team
  * @version V3.3.0
  * @date    04/16/2010
  * @brief   Main program body
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/

#include "stm324xg_eval.h"
#include "Printf.h"
#include <stdio.h>

/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup USART_Printf
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#ifndef EVAL_COM1_STR
	    #define EVAL_COM1_STR                    "USART1"
#endif
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
unsigned char USART1_putchar(unsigned char ch);
unsigned char USART2_putchar(unsigned char ch);

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
  
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
	
/*************************************************
Function: void USART_Config(void)      
Description: USART配置函数             
Input: 无                              
Output:无                              
Return:无                              
*************************************************/  
void Printf_Init(void)
{
			/*定义了一个GPIO_InitStructure的结构体，方便一下使用 */
			GPIO_InitTypeDef GPIO_InitStructure;
			USART_InitTypeDef USART_InitStructure;
			USART_ClockInitTypeDef USART_ClockInitStruct;
			/* 初始化GPIOG时钟*/
			
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); //开启USART6时钟
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  //开启GPIOC时钟
			GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);//这相当于M3的开启复用时钟？只配置复用的引脚，
			GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);//               
			/*配置GPIOC*/
			GPIO_StructInit(&GPIO_InitStructure);      //缺省值填入

			/*配置GPIOC_Pin6为TX输出*/
			GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
			GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;     //设置为复用，必须为AF，OUT不行
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOA,&GPIO_InitStructure);

			/*配置GPIOC_Pin7为RX输入*/
			GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
			GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;     //这也必须为复用，与M3不同！
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOA,&GPIO_InitStructure);

			/*IO引脚复用功能设置，与之前版本不同*/
			/*配置USART6*/
			USART_StructInit(&USART_InitStructure);
			USART_InitStructure.USART_BaudRate =115200;
			USART_InitStructure.USART_WordLength = USART_WordLength_8b;
			USART_InitStructure.USART_StopBits = USART_StopBits_1;
			USART_InitStructure.USART_Parity = USART_Parity_No;
			USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
			USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
			USART_Init(USART1, &USART_InitStructure);
			USART_ClockStructInit(&USART_ClockInitStruct);    //之前没有填入缺省值，是不行的
			USART_ClockInit(USART1, &USART_ClockInitStruct);

			USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //使能 USART1接收中断
			USART_Cmd(USART1, ENABLE);         //使能 USART6 
			
			
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE); //开启USART6时钟
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); //开启GPIOC时钟
			GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);//这相当于M3的开启复用时钟？只配置复用的引脚，
			GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);// 
			/*配置GPIOC*/
			GPIO_StructInit(&GPIO_InitStructure); //缺省值填入
			/*配置GPIOC_Pin6为TX输出*/
			GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
			GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF; //设置为复用，必须为AF，OUT不行
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOC,&GPIO_InitStructure);
			/*配置GPIOC_Pin7为RX输入*/
			GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;
			GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF; //这也必须为复用，与M3不同！
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOC,&GPIO_InitStructure);
			/*IO引脚复用功能设置，与之前版本不同*/
			/*配置USART6*/
			USART_StructInit(&USART_InitStructure);
			USART_InitStructure.USART_BaudRate =115200;
			USART_InitStructure.USART_WordLength = USART_WordLength_8b;
			USART_InitStructure.USART_StopBits = USART_StopBits_1;
			USART_InitStructure.USART_Parity = USART_Parity_No;
			USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
			USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
			USART_Init(USART6, &USART_InitStructure);
			USART_ClockStructInit(&USART_ClockInitStruct); //之前没有填入缺省值，是不行的
			USART_ClockInit(USART6, &USART_ClockInitStruct);
			USART_ITConfig(USART6, USART_IT_RXNE, ENABLE); //使能 USART6中断
			USART_Cmd(USART6, ENABLE); //使能 USART6 		
}

void USART6_Puts(char * str)
{
		 while (*str)
		 {
		 USART_SendData(USART6, *str++);

		 /* Loop until the end of transmission */
		 while (USART_GetFlagStatus(USART6, USART_FLAG_TXE) == RESET); //详见英文参考的521页，当TXE被置起时，一帧数据传输完成
		 }
}



/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
		USART_SendData(USART6, (uint8_t) ch);

		/* Loop until the end of transmission */
		while (USART_GetFlagStatus(USART6, USART_FLAG_TC) == RESET)
		{}

		return ch;
}

/**
  * @}
  */ 
unsigned char USART2_putchar(unsigned char ch)
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(EVAL_COM2, (uint8_t) ch); 

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(EVAL_COM2, USART_FLAG_TC) == RESET)
  {
  
  }
  return ch;
}

unsigned char USART1_putchar(unsigned char ch)
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(EVAL_COM1, (uint8_t) ch); 

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TC) == RESET)
  {
  
  }
  return ch;
}
/**
  * @}
  */ 

/**
  * @brief  Test to see if a key has been pressed on the HyperTerminal
  * @param  key: The key pressed
  * @retval 1: Correct
  *         0: Error
  */
uint32_t USART2_ValidInput(void)
{

  if ( USART_GetFlagStatus(EVAL_COM2, USART_FLAG_RXNE) != RESET)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

/**
  * @brief  Test to see if a key has been pressed on the HyperTerminal
  * @param  key: The key pressed
  * @retval 1: Correct
  *         0: Error
  */
uint32_t USART1_ValidInput(void)
{

  if ( USART_GetFlagStatus(EVAL_COM1, USART_FLAG_RXNE) != RESET)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

uint8_t USART2_getchar(void)
{
  uint8_t key = 0;

  /* Waiting for user input */
  while (1)
  {
    if ( USART_GetFlagStatus(EVAL_COM2, USART_FLAG_RXNE) != RESET)
    {
      key = (uint8_t)EVAL_COM2->DR & 0xFF;
      break;
    }
  }
  return key;
}

uint8_t USART1_getchar(void)
{
  uint8_t key = 0;

  /* Waiting for user input */
  while (1)
  {
    if ( USART_GetFlagStatus(EVAL_COM1, USART_FLAG_RXNE) != RESET)
    {
      key = (uint8_t)EVAL_COM1->DR & 0xFF;
      break;
    }
  }
  return key;
}

/**
  * @brief  Get Input string from the HyperTerminal
  * @param  buffP: The input string
  * @retval None
  */
uint32_t USART1_gets_nowait (uint8_t * buffP, uint32_t length)
{
    static uint32_t bytes_read = 0;
    uint8_t c = 0;
  
    if(USART1_ValidInput())
    {
        c = USART1_getchar();
        if (c == '\r')
        {
          USART1_putchar('\n');
          USART1_putchar('\r');          
          buffP[bytes_read] = '\0';
          bytes_read = 0;

          return 1;
        }

        if (c == '\b') /* Backspace */
        {
          if (bytes_read > 0)
          {
            USART1_putchar('\b');
            USART1_putchar(' ');
            USART1_putchar('\b');
            bytes_read--;
          }
          return 0;          
        }
        //if (bytes_read >= (CMD_STRING_SIZE))
        if (bytes_read >= (length))
        {
          printf("Command string size overflow\r\n");
          bytes_read = 0;
          return 0;
        }
        if (c >= 0x20 && c <= 0x7E)
        {
          buffP[bytes_read++] = c;
          USART1_putchar(c);
        }
    }
    
    return 0;
}

uint32_t USART2_gets_nowait (uint8_t * buffP, uint32_t length)
{
    static uint32_t bytes_read = 0;
    uint8_t c = 0;
  
    if(USART2_ValidInput())
    {
        c = USART2_getchar();
        if (c == '\r')
        {
          USART2_putchar('\n');
          USART2_putchar('\r');          
          buffP[bytes_read] = '\0';
          bytes_read = 0;

          return 1;
        }

        if (c == '\b') /* Backspace */
        {
          if (bytes_read > 0)
          {
            USART2_putchar('\b');
            USART2_putchar(' ');
            USART2_putchar('\b');
            bytes_read--;
          }
          return 0;          
        }
        //if (bytes_read >= (CMD_STRING_SIZE))
        if (bytes_read >= (length))
        {
          //printf("Command string size overflow\r\n");
          bytes_read = 0;
          return 0;
        }
        if (c >= 0x20 && c <= 0x7E)
        {
          buffP[bytes_read++] = c;
          USART2_putchar(c);
        }
    }
    
    return 0;
}
/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
