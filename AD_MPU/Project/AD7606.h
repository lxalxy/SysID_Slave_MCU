#ifndef AD7656_H
#define AD7656_H
#include <stm32f4xx.h>

		//PB13 SCK RD
		//PB14 MISO DB7
		//PB15 MOSI

#define AD_GPIO GPIOB
#define AD_RESET GPIO_Pin_0
#define AD_CNVST GPIO_Pin_1
#define AD_BUSY GPIO_Pin_2
#define AD_CS GPIO_Pin_10
#define AD_RAGE GPIO_Pin_4
#define AD_OS0 GPIO_Pin_5
#define AD_OS1 GPIO_Pin_6
#define AD_OS2 GPIO_Pin_7
#define AD_CNVSTB GPIO_Pin_8

#define SET_RESET() 	GPIO_SetBits(AD_GPIO, AD_RESET)
#define CLR_RESET() 	GPIO_ResetBits(AD_GPIO, AD_RESET)
#define SET_CS() 			GPIO_SetBits(AD_GPIO, AD_CS)
#define CLR_CS() 			GPIO_ResetBits(AD_GPIO, AD_CS)
#define SET_CNVST() 	GPIO_SetBits(AD_GPIO, AD_CNVST)
#define CLR_CNVST() 	GPIO_ResetBits(AD_GPIO, AD_CNVST)
#define RAGE10() 			GPIO_SetBits(AD_GPIO, AD_RAGE)
#define RAGE5() 			GPIO_resetBits(AD_GPIO, AD_RAGE)
#define SET_OS0() 		GPIO_SetBits(AD_GPIO, AD_OS0)
#define CLR_OS0() 		GPIO_ResetBits(AD_GPIO, AD_OS0)
#define SET_OS1() 		GPIO_SetBits(AD_GPIO, AD_OS1)
#define CLR_OS1() 		GPIO_ResetBits(AD_GPIO, AD_OS1)
#define SET_OS2()		 	GPIO_SetBits(AD_GPIO, AD_OS2)
#define CLR_OS2()		 	GPIO_ResetBits(AD_GPIO, AD_OS2)
#define SET_CNVSTB() 	GPIO_SetBits(AD_GPIO, AD_CNVSTB)
#define CLR_CNVSTB() 	GPIO_ResetBits(AD_GPIO, AD_CNVSTB)

void AD7606_Init(void);
u16 SPI_ReadFromAD7656(void);
void AD7606_Reset(void);
void AD_GPIO_Init(void);
void send_ADValue(u16 ADValue);

void SPIx_Init(void);			 //初始化SPI口
u16 SPIx_ReadWriteByte(u16 TxData);//SPI总线读写一个字节


#endif



