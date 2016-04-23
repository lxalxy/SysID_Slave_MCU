/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/main.c 
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    13-April-2012
  * @brief   Main program body
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
#include "main.h"
#include "ff.h"
#include "stm324x7i_eval_sdio_sd.h"
#include "peripherals.h"
#include "delay.h"
#include "stdio.h"
#include "AD7606.h"
#include "I2C.h"
#include "MPU9250.h"
#include "Queue.h"

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define BOOL bool
#define SD_WRITE_BUFF_SIZE 0x400

FATFS fs;//逻辑磁盘工作区.	 
FIL file;	  		//文件1

unsigned int res,i,bw,br;
extern __IO uint16_t ADCoverVaule;
extern u8 KeyFlag;
extern u8 KeyFlag;
extern u8 ReKeyFlag;
u8 current_state=0;
int Adc;
u16 TimerCounter = 0;
float IMU_omega = 0.0;

char writebuf[50];
char readbuf[120];
char filename[12];
u8 NewFileFlag = 1;
u8 FirstFileFlag = 1;
u16 ADValue = 0;
float Vout = 0.0;
u16 temp_f_sync =0;
char isReady = 0;

char check1,check2,check3 = 0;

u32 num = 0;

u16 TimerPeriod = 250;

static uint8_t SD_WRITE_BUFF[SD_WRITE_BUFF_SIZE] = {0};
static QUEUE8_TYPE SD_WRITE_QUEUE = {0};


/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{       
		u8 SecondsTemp=0;
		u8 temp = 0;
		u16 TimerTemp = 0;
		TimerTemp = 10000 / TimerPeriod;
		delay_init(168);
		Peripherals_Init();
		Printf_Init(); //115200 baud
		Timer4_Init(TimerTemp-1,8400-1);//读取中断，定时器时钟84M，分频系数8400，所以84M/8400/200=50hz	
		//Timer3_Init(10-1,8400-1);//按键中断，定时器时钟84M，分频系数8400，所以84M/8400/10=1000hz		
		SD_NVIC_Configuration();
		SPIx_Init();
		AD_GPIO_Init();
		AD7606_Reset();
		AD7606_Init();
		//GPIO_ResetBits(GPIOB, GPIO_Pin_9);
		IIC_Init();
		delay_ms(10);
		MPU9250_Init();
		delay_ms(10);
		
		IIC2_Init();
		delay_ms(10);
		MPU9250_2_Init();
		delay_ms(10);
		
		IIC3_Init();
		delay_ms(10);
		MPU9250_3_Init();
		delay_ms(10);
		
		check1 = MPU9250_Check();
		check2 = MPU9250_2_Check();
		check3 = MPU9250_3_Check();
		
		QUEUE_PacketCreate(&SD_WRITE_QUEUE, SD_WRITE_BUFF, SD_WRITE_BUFF_SIZE);

		
	
		if(check1&check2&check3)
		{
			GPIO_ResetBits(GPIOB, GPIO_Pin_9);
		}
		else
		{
			GPIO_SetBits(GPIOB, GPIO_Pin_9);
		}
		
		res = SD_Init();
		//printf("The result of SD init is %d \n", res);
		res = f_mount(0, &fs); 					//挂载SD卡
		printf("The result of SD load is %d \n", res);	
		sprintf(filename,"TEST_%d.TXT", 0);

		res = f_open(&file, filename, FA_WRITE | FA_CREATE_ALWAYS);
		printf("The result of open file is %d \n", res);	
		if(res == 0)
				isReady = 1;
		
//		res = f_printf(&file,"Hello world!!! On SD card!\n");
//		res = f_close(&file);
		
		while (1)
		{
				QUEUE_PacketOut(&SD_WRITE_QUEUE, readbuf, 100);
				if(sizeof(readbuf) > 0)
				{
						res = f_printf(&file, readbuf);
					//	f_sync(&file);
						for(temp = 0; temp < sizeof(readbuf); temp++)
						{
									readbuf[temp]=0x00;
						}
				}
				if(temp_f_sync > TimerPeriod)
				{
							f_sync(&file);
							temp_f_sync = 0;
							SecondsTemp++;
				}
				
				if(SecondsTemp > 60)
				{
							f_close(&file);
							SecondsTemp = 0;
							isReady = 0;
							GPIO_SetBits(GPIOB, GPIO_Pin_9);
				}

				//printf("The result of SD load is %d \n", res);		
			//	exf_getfree("0", &total, &free);	//得到SD卡的总容量和剩余容量
			//	printf("The total of SD is %d, and free is %d \n", total, free);
		}
}

void TIM4_IRQHandler(void)
{
		char temp = 0;
		if(TIM_GetITStatus(TIM4, TIM_IT_Update)==SET) //溢出中断
		{
//				USART6_2_Byte(READ_MPU9250_ACCEL(3));
//				USART6_2_Byte(READ_MPU9250_2_ACCEL(3));
//				USART6_2_Byte(READ_MPU9250_3_ACCEL(3));
					if(isReady)
					{
							temp_f_sync++;
							num++;
							for(temp = 0; temp < sizeof(writebuf); temp++)
							{
										writebuf[temp]=0x00;
							}
							sprintf(writebuf, "%d %d %d %d %d\n", READ_MPU9250_ACCEL(1), READ_MPU9250_ACCEL(2), READ_MPU9250_2_ACCEL(1), READ_MPU9250_3_ACCEL(2), num);
							//sprintf(buf, "%d %d %d %d\n", num, READ_MPU9250_ACCEL(3), READ_MPU9250_2_ACCEL(3), READ_MPU9250_3_ACCEL(3));
							//res=f_write(&file, buf, sizeof(buf), &bw);
							QUEUE_PacketIn(&SD_WRITE_QUEUE, writebuf, 30);
							//QUEUE_PacketOut(&SD_WRITE_QUEUE, buf, 3);
							//res = f_printf(&file,"Hello world!!! On SD card!\n");
							//f_close(&file);
							//isReady = 0;

					}
			
		}
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //清除中断标志位
}




#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
