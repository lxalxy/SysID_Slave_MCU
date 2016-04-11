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
#include "Mti.h"
#include "A2P_ADC.h"
#include "sdio_sdcard.h"
#include "peripherals.h"
#include "delay.h"
#include "malloc.h" 
#include "RTC.h"
#include "stdio.h"
//#include "ads1256.h"
#include "AD7606.h"

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
struct Mti_Data RecievedData; 
extern u8 KeyFlag;
extern u8 ReKeyFlag;
u8 current_state=0;
RTC_TimeStamp SetMoment;
RTC_TimeStamp CurrentMoment;
int Adc;
u16 TimerCounter = 0;
float IMU_omega = 0.0;

char buf[50];
u8 NewFileFlag = 1;
u8 FirstFileFlag = 1;
u8 isWriting = 0;
u16 ADValue = 0;
float Vout = 0.0;

u16 temp_f_sync = 0;

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{       
		u32 total,free;
		char temp = 0;
		//u8 IS_RUNNING=0;
		u16 File_Num = 0;
		u16 UsartMessageSum = 0;
		u8 UsartTemp = 0;
		//char buf[50];
		//RTC_Config(); //RTC 初始化并启动RTC
		delay_init(168);
		Peripherals_Init();
		Mti_Init(&RecievedData);
		//A2P_ADC_Init();
		//A2P_ADCDMA_Init();
		Printf_Init(); //115200 baud
		Timer4_Init(5-1,8400-1);//读取中断，定时器时钟84M，分频系数8400，所以84M/8400/200=50hz	
		Timer3_Init(10-1,8400-1);//按键中断，定时器时钟84M，分频系数8400，所以84M/8400/10=1000hz	
		my_mem_init(SRAMIN);		//初始化内部内存池 
		my_mem_init(SRAMCCM);		//初始化CCM内存池
		exfuns_init();							//为fatfs相关变量申请内存				
//		Init_ADS1256_GPIO(); //初始化ADS1256 GPIO管脚 
//		delay_ms(50);
//		ADS1256_Init();
		SPIx_Init();
		AD_GPIO_Init();
		AD7606_Reset();
		AD7606_Init();
		GPIO_SetBits(GPIOB, GPIO_Pin_9);


//		
		//RTC_TimeStampData(&SetMoment); //RTC 设置的数据
		//RTC_TimeStampWrite(SetMoment); //RTC 日期时间设置
//	
		//ADC_SoftwareStartConv(ADC1);     //如果不是外部触发则必须软件开始转换
		

		
		
		while (1)
		{
				
//				GPIO_ToggleBits(GPIOB, GPIO_Pin_9);
//				delay_ms(1000);
			
			
			
				if(KeyFlag)
				{
						KeyFlag = 0;  
						if(FirstFileFlag)
						{
								res = SD_Init();
								//printf("The result of SD init is %d \n", res);
								res = f_mount(fs[0],"0:",1); 					//挂载SD卡
								//printf("The result of SD load is %d \n", res);		
							//	exf_getfree("0", &total, &free);	//得到SD卡的总容量和剩余容量
							//	printf("The total of SD is %d, and free is %d \n", total, free);
								FirstFileFlag = 0;
						}
						if(NewFileFlag)
						{
								sprintf(buf,"TEST_%d.TXT", File_Num);
								res = f_open(file, buf, FA_WRITE | FA_CREATE_ALWAYS);
//								for(temp = 0; temp < sizeof(buf); temp++)
//								{
//										buf[temp]=0;
//								}
								if(res == 0)
								{
										GPIO_ResetBits(GPIOB, GPIO_Pin_9);
										File_Num++;
										NewFileFlag = 0;
										isWriting = 1;
								}
								else
								{
										break;
								}													
						}
						else
						{
								res = f_close(file);
								if(res == 0)
								{
										GPIO_SetBits(GPIOB, GPIO_Pin_9);
										//current_state = 0;
										NewFileFlag = 1;
										isWriting = 0;
								}
								else
								{
										break;
								}
						}
				}
				
				if(packageflag == 1)
				{
						for(UsartTemp = 0; UsartTemp < 12; UsartTemp++)
						{
								UsartMessageSum += rxbuf[UsartTemp];
						}
						if(((UsartMessageSum + RecievedData.CHECKSUM + RecievedData.LEN + 0xFF + 0x32) & (0x00FF)) == 0)
						{
								//RecievedData.DATA[0].i = (rxbuf[0]<<24) + (rxbuf[1]<<16) + (rxbuf[2]<<8) + (rxbuf[3]);
								//RecievedData.DATA[1].i = (rxbuf[4]<<24) + (rxbuf[5]<<16) + (rxbuf[6]<<8) + (rxbuf[7]);
								RecievedData.DATA[2].i = (rxbuf[8]<<24) + (rxbuf[9]<<16) + (rxbuf[10]<<8) + (rxbuf[11]);
							 // printf("%f\n", RecievedData.DATA[2].f);
						}
						UsartMessageSum = 0;
						for(UsartTemp = 0; UsartTemp < 12; UsartTemp++)
						{
								rxbuf[UsartTemp]=0;
						}
						packageflag = 0;
				}





				//printf("%d\n", RecievedData.DATA[2].i);
//				switch(current_state)
//				{
//					case 0:
//					{
//							res = SD_Init();
//					
//							printf("The result of SD init is %d \n", res);
//						
//							res = f_mount(fs[0],"0:",1); 					//挂载SD卡 
//							
//							printf("The result of SD load is %d \n", res);

//							exf_getfree("0", &total, &free);	//得到SD卡的总容量和剩余容量

//							printf("The total of SD is %d, and free is %d \n", total, free);
//							/*
//							sprintf(buf,"TEST_ADC_%d.TXT", File_Num);
//						
//							printf("\r\nCreate a new file.");
//							res = f_open(file, buf, FA_WRITE | FA_CREATE_ALWAYS);
//							printf("\r\nf_open = <%d>",res);
//							
//							if(res == 0)
//									File_Num++;
//							*/
//							
//							current_state = 4;
//							break;
//					}
//					case 1:
//					{
//							if (IS_RUNNING == 0)
//									current_state = 5;
//							else if (IS_RUNNING == 1)
//									current_state = 3;
//							break;
//					}
//					case 2:
//					{
//							IS_RUNNING = 1;
//							CurrentMoment = RTC_TimeStampRead();
//							sprintf(buf," %x-%x-%x,%x:%x:%x\t%d\r\n", CurrentMoment.year, CurrentMoment.month, CurrentMoment.day, CurrentMoment.hour, CurrentMoment.min, CurrentMoment.sec, ADCoverVaule); //ADCoverVaule*3300/4096
//							//printf("Wrting : %s\n",buf);
//							res=f_write(file, buf, sizeof(buf), &bw);		
//						//	printf("The written result is: %d\n", res);
//							break;
//					}
//					case 3:
//					{
//							printf("\r\nClose the file.\r\n");
//							res = f_close(file);
//							printf("\r\nf_close = <%d>\r\n",res);
//							IS_RUNNING = 0;
//							current_state = 4;
//							break;
//					}
//					case 4:
//					{
//							break;
//					}
//					case 5:
//					{
//							CurrentMoment = RTC_TimeStampRead();
//							sprintf(buf,"TEST_ADC_%x_%x_%x_%d.TXT", CurrentMoment.hour, CurrentMoment.min, CurrentMoment.sec, File_Num); //ADCoverVaule*3300/4096
//							printf("\r\nCreate a new file.");
//							res = f_open(file, buf, FA_WRITE | FA_CREATE_ALWAYS);
//							printf("\r\nf_open = <%d>",res);
//							
//							if(res == 0)
//									File_Num++;
//							
//							current_state = 2;					
//					}
//					default:
//							break;
//				}
		}
}

void TIM4_IRQHandler(void)
{
		char temp = 0;
		if(TIM_GetITStatus(TIM4, TIM_IT_Update)==SET) //溢出中断
		{
//				TimerCounter++;                         //有键按下，计数kcount加1
//				if(TimerCounter >= 2)
//				{
//						IMU_omega = RecievedData.DATA[2].f;
//						TimerCounter = 0;
//				}
				if(isWriting)
				{
						temp_f_sync++;
						ADValue = SPI_ReadFromAD7656();
						for(temp = 0; temp < sizeof(buf); temp++)
						{
								buf[temp]=0;
						}
						sprintf(buf, "%d\n", ADValue); //Volts = Adc*0.000000598;
						//sprintf(buf, "%f %d\n", RecievedData.DATA[2].f, ADValue); //Volts = Adc*0.000000598;
						res=f_write(file, buf, sizeof(buf), &bw);
						if(temp_f_sync > 2000)
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
