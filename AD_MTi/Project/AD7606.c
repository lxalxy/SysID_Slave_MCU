/********************************************************************************
 Author : CAST (China Applications Support Team) 

 Date : 3-29-2011

 File name : AD7656.c

 Description : Using ADuC7026 to control AD7606 in software SPI form through GPIO

 Hardware plateform : ADuC7026 + AD7656	
********************************************************************************/
#include <stm32f4xx.h>
#include <stdio.h>
#include "AD7606.h"
#include "delay.h"

#include<stdbool.h>


void SPIx_Init(void)
{
		SPI_InitTypeDef  SPI_InitStructure;
		GPIO_InitTypeDef GPIO_InitStructure;
		/****Initial SPI2******************/

		/* Enable SPI2 and GPIOB clocks */
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

		/* Configure SPI2 pins: NSS, SCK, MISO and MOSI */

		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  //���ų�ʼ��
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB,&GPIO_InitStructure);
		
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_SPI2);  //�����ŵĸ��ù���
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_SPI2);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_SPI2);
	
		/* SPI2 configuration */ 
		SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //SPI1����Ϊ����ȫ˫��
		SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                    //����SPI2Ϊ��ģʽ
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;                  //SPI���ͽ���8λ֡�ṹ
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;                   //����ʱ���ڲ�����ʱ��ʱ��Ϊ�͵�ƽ
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;                 //��һ��ʱ���ؿ�ʼ��������
		SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                  //NSS�ź��������ʹ��SSIλ������
		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64; //���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ8
		SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;       //���ݴ����MSBλ��ʼ
		SPI_InitStructure.SPI_CRCPolynomial = 7;         //CRCֵ����Ķ���ʽ
		SPI_Init(SPI2, &SPI_InitStructure);
		/* Enable SPI2  */
		SPI_Cmd(SPI2, ENABLE);  
}   


//SPIx ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u16 SPIx_ReadWriteByte(u16 TxData)
{		
		u8 retry = 0;				 	
		while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
		{
				retry++;
				if(retry > 200)
						return 0;
		}
		SPI_I2S_SendData(SPI2, TxData); //ͨ������SPIx����һ������
		retry = 0;

		while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) //���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
		{
				retry++;
				if(retry > 200)
						return 0;
		}
		return SPI_I2S_ReceiveData(SPI2); //����ͨ��SPIx������յ�����
}                              

 


void AD_GPIO_Init(void)
{
		GPIO_InitTypeDef  GPIO_InitStructure;

		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_10|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  		 //�������
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;      
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void AD7606_Init()
{
		
	
	//	RAGE10();
		delay_ms(1);
		CLR_OS2();
		SET_OS1();
		SET_OS0();
		delay_ms(1);

}


void AD7606_Reset(void)
{
		CLR_RESET();
		delay_us(1);
		SET_RESET();
		delay_us(1);
		CLR_RESET();
		delay_us(1);
		SET_CS();
}


//---------------------------------
//ReadFromAD7606();
//-------------------------------------------------------------------------------
//Function that reads from the AD7606 via the SPI port. 
//--------------------------------------------------------------------------------
u16 SPI_ReadFromAD7656(void)
{
		bool isBUSY;
		u16 ADtemp[8];
    SET_CS();
    delay_us(2);
		SET_CNVST();
		SET_CNVSTB();
		delay_us(1);

		CLR_CNVST();
		CLR_CNVSTB();
		delay_us(1);
		SET_CNVST();
		SET_CNVSTB();
    delay_us(1);
		isBUSY = GPIO_ReadInputDataBit(AD_GPIO, AD_BUSY);
		while(isBUSY)
		{
				delay_us(1);
				isBUSY = GPIO_ReadInputDataBit(AD_GPIO, AD_BUSY);
		}
    CLR_CS();

		ADtemp[0] = SPIx_ReadWriteByte(0xaaaa);
		ADtemp[1] = SPIx_ReadWriteByte(0xaaaa);
		ADtemp[2] = SPIx_ReadWriteByte(0xaaaa);
		ADtemp[3] = SPIx_ReadWriteByte(0xaaaa);
	
		ADtemp[4] = SPIx_ReadWriteByte(0xaaaa);
		ADtemp[5] = SPIx_ReadWriteByte(0xaaaa);
		ADtemp[6] = SPIx_ReadWriteByte(0xaaaa);
		ADtemp[7] = SPIx_ReadWriteByte(0xaaaa);
		
		SET_CS();
		return ADtemp[0];
}









