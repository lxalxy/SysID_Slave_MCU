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

//这是dma的

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "ff.h"
//#include "Mti.h"
//#include "A2P_ADC.h"
#include "sdio_sdcard.h"
#include "peripherals.h"
#include "delay.h"
#include "malloc.h" 
//#include "RTC.h"
#include "stdio.h"
//#include "ads1256.h"
#include "AD7606.h"
#include "I2C.h"
#include "MPU9250.h"

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define BOOL bool


FATFS *fs[_VOLUMES];//逻辑磁盘工作区.	 
FIL *file;	  		//文件1
FIL *ftemp;	  		//文件2.
UINT br,bw;			//读写变量
FILINFO fileinfo;	//文件信息
DIR dir;  			//目录

u8 *fatbuf;			//SD卡数据缓存区
unsigned int res,i,bw,br;
extern __IO uint16_t ADCoverVaule;
extern u8 KeyFlag;
extern u8 UsartMessage;
extern unsigned char packageflag;//是否接收到一个完整的数据包标志
extern unsigned char rxbuf[20];//接收数据的缓冲区
extern u8 KeyFlag;
extern u8 ReKeyFlag;
u8 current_state=0;
int Adc;
u16 TimerCounter = 0;
float IMU_omega = 0.0;

char buf[50];
u8 NewFileFlag = 1;
u8 FirstFileFlag = 1;
u16 ADValue = 0;
float Vout = 0.0;
char temp = 0;
u16 temp_f_sync =0;
char isReady = 0;

char check1,check2,check3 = 0;

u32 num = 0;

u16 TimerPeriod = 500;


/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{       
		u16 TimerTemp = 0;
		TimerTemp = 10000 / TimerPeriod;
		delay_init(168);
		Peripherals_Init();
		Printf_Init(); //115200 baud
		Timer4_Init(TimerTemp-1,8400-1);//读取中断，定时器时钟84M，分频系数8400，所以84M/8400/200=50hz	
		//Timer3_Init(10-1,8400-1);//按键中断，定时器时钟84M，分频系数8400，所以84M/8400/10=1000hz		
		my_mem_init(SRAMIN);		//初始化内部内存池 
		my_mem_init(SRAMCCM);		//初始化CCM内存池
		exfuns_init();							//为fatfs相关变量申请内存	
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
		res = f_mount(fs[0],"0:",1); 					//挂载SD卡
		sprintf(buf,"TEST_%d.TXT", 0);
		res = f_open(file, buf, FA_WRITE | FA_CREATE_ALWAYS);
		if(res == 0)
				isReady = 1;
		
		while (1)
		{

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
							for(temp = 0; temp < sizeof(buf); temp++)
							{
										buf[temp]=0;
							}
							sprintf(buf, "%d %d %d %d %d\n", READ_MPU9250_ACCEL(1), READ_MPU9250_ACCEL(2), READ_MPU9250_2_ACCEL(1), READ_MPU9250_3_ACCEL(2), num);
							//sprintf(buf, "%d %d %d %d\n", num, READ_MPU9250_ACCEL(3), READ_MPU9250_2_ACCEL(3), READ_MPU9250_3_ACCEL(3));
							res=f_write(file, buf, sizeof(buf), &bw);
							if(temp_f_sync > TimerPeriod)
							{
										f_sync(file);
										temp_f_sync = 0;
							}
					}
			
		}
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //清除中断标志位
}


u8 exf_getfree(u8 *drv,u32 *total,u32 *free)
{
	FATFS *fs1;
	u8 res;
    u32 fre_clust=0, fre_sect=0, tot_sect=0;
    //得到磁盘信息及空闲簇数量
    res =(u32)f_getfree((const TCHAR*)drv, (DWORD*)&fre_clust, &fs1);
    if(res==0)
	{											   
	    tot_sect=(fs1->n_fatent-2)*fs1->csize;	//得到总扇区数
	    fre_sect=fre_clust*fs1->csize;			//得到空闲扇区数	   
#if _MAX_SS!=512				  				//扇区大小不是512字节,则转换为512字节
		tot_sect*=fs1->ssize/512;
		fre_sect*=fs1->ssize/512;
#endif	  
		*total=tot_sect>>1;	//单位为KB
		*free=fre_sect>>1;	//单位为KB 
 	}
	return res;
}	

u8 exfuns_init(void)
{
	u8 i;
	for(i=0;i<_VOLUMES;i++)
	{
		fs[i]=(FATFS*)mymalloc(SRAMIN,sizeof(FATFS));	//为磁盘i工作区申请内存	
		if(!fs[i])break;
	}
	file=(FIL*)mymalloc(SRAMIN,sizeof(FIL));		//为file申请内存
	ftemp=(FIL*)mymalloc(SRAMIN,sizeof(FIL));		//为ftemp申请内存
	fatbuf=(u8*)mymalloc(SRAMIN,512);				//为fatbuf申请内存
	if(i==_VOLUMES&&file&&ftemp&&fatbuf)return 0;  //申请有一个失败,即失败.
	else return 1;	
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
