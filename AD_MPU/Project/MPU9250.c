#include "I2C.h"
#include "MPU9250.h"
#include "delay.h"


//初始化MPU9250，根据需要请参考pdf进行修改************************
void MPU9250_Init(void)
{
//		u8 data = 0;
/*
   Single_Write(GYRO_ADDRESS,PWR_M, 0x80);   //
   Single_Write(GYRO_ADDRESS,SMPL, 0x07);    //
   Single_Write(GYRO_ADDRESS,DLPF, 0x1E);    //±2000°
   Single_Write(GYRO_ADDRESS,INT_C, 0x00 );  //
   Single_Write(GYRO_ADDRESS,PWR_M, 0x00);   //
*/
//		MPU9250_WriteOneByte(GYRO_ADDRESS,PWR_MGMT_1, 0x00);	//解除休眠状态
//		MPU9250_WriteOneByte(GYRO_ADDRESS,SMPLRT_DIV, 0x07);
//		MPU9250_WriteOneByte(GYRO_ADDRESS,CONFIG, 0x06);
//		MPU9250_WriteOneByte(GYRO_ADDRESS,GYRO_CONFIG, 0x10);
//		MPU9250_WriteOneByte(GYRO_ADDRESS,ACCEL_CONFIG, 0x01);
		//MPU9250_WriteOneByte(GYRO_ADDRESS,ACCEL_CONFIG2, 0x08);
	
		MPU9250_WriteOneByte(GYRO_ADDRESS,PWR_MGMT_1, 0x80);	//RESET
		delay_ms(100);
		MPU9250_WriteOneByte(GYRO_ADDRESS,PWR_MGMT_1, 0x03);	//CLOCK_PLL
		delay_ms(1);
		MPU9250_WriteOneByte(GYRO_ADDRESS,PWR_MGMT_2, XYZ_GYRO & XYZ_ACCEL);	//Set Sensors
		delay_ms(1);
		MPU9250_WriteOneByte(GYRO_ADDRESS,SMPLRT_DIV, 0x00); //SAMPLE_RATE = Internal_Sample_Rate / (1 + SMPLRT_DIV)
		delay_ms(1);
		MPU9250_WriteOneByte(GYRO_ADDRESS,GYRO_CONFIG, 0x18);//MPU9250 Set Full Scale Gyro Range;Fchoice_b[1:0] = [00] enable DLPF
		delay_ms(1);
		MPU9250_WriteOneByte(GYRO_ADDRESS,ACCEL_CONFIG, 0x00);//MPU9250 Set Full Scale Accel Range PS:2G
		delay_ms(1);
		MPU9250_WriteOneByte(GYRO_ADDRESS,ACCEL_CONFIG2, 0x08);//MPU9250 Set Accel DLPF
		delay_ms(1);
		MPU9250_WriteOneByte(GYRO_ADDRESS,CONFIG, 0x03);//MPU9250 Set Gyro DLPF
		delay_ms(1);
		
	
//		if(MPU9250_Check())
//		{
//			GPIO_ResetBits(GPIOB, GPIO_Pin_9);
//		}
//		else
//		{
//			GPIO_SetBits(GPIOB, GPIO_Pin_9);
//		}
  //----------------
//	Single_Write(GYRO_ADDRESS,0x6A,0x00);//close Master Mode	

}
	

uint8_t MPU9250_Check(void) 
{
		uint8_t temp = 0;
		temp = MPU9250_ReadOneByte(GYRO_ADDRESS, WHO_AM_I);
   	if(WHO_AM_I_VAL == temp)     	
		{
   		return 1;
   	}
   	else 
   	{
   		return 0;
   	}	
}


//在指定地址读出一个数据
//ReadAddr:开始读数的地址  
//返回值  :读到的数据
uint8_t MPU9250_ReadOneByte(unsigned char SlaveAddress, unsigned char REG_Address)
{				  
		u8 temp=0;
    IIC_Start();
		IIC_Send_Byte(SlaveAddress);	 //发送设备地址
		IIC_Wait_Ack();
		IIC_Send_Byte(REG_Address);   //发送低起始地址
		IIC_Wait_Ack();
		IIC_Start();   
		IIC_Send_Byte(SlaveAddress + 1);  	   
		IIC_Wait_Ack();
    temp = IIC_Read_Byte(0);		   
    IIC_Stop();//产生一个停止条件	    
		return temp;
}

//在指定地址写入一个数据
//WriteAddr  :写入数据的目的地址    
//DataToWrite:要写入的数据
void MPU9250_WriteOneByte(unsigned char SlaveAddress, unsigned char REG_Address, unsigned char REG_data)
{				   	  	    																 
    IIC_Start();  
		IIC_Send_Byte(SlaveAddress);	   
		IIC_Wait_Ack();
		IIC_Send_Byte(REG_Address); 
		IIC_Wait_Ack();	   
    IIC_Send_Byte(REG_data);
		IIC_Wait_Ack(); 	 										  		   		    	   
    IIC_Stop();//产生一个停止条件 
		//delay_ms(2);	 
}	




//******读取MPU9250数据****************************************
int16_t READ_MPU9250_ACCEL(uint8_t axis)
{ 
		unsigned char BUF[10];
		uint16_t output;

		switch(axis)
		{
				case 1:
				{
						BUF[0] = MPU9250_ReadOneByte(ACCEL_ADDRESS,ACCEL_XOUT_L); 
						BUF[1] = MPU9250_ReadOneByte(ACCEL_ADDRESS,ACCEL_XOUT_H);
						output = (BUF[1]<<8)|BUF[0];
						//output = output * 2 / 32768; 						   //读取计算X轴数据
						break;
				}
				case 2:
				{
						BUF[2] = MPU9250_ReadOneByte(ACCEL_ADDRESS,ACCEL_YOUT_L);
						BUF[3] = MPU9250_ReadOneByte(ACCEL_ADDRESS,ACCEL_YOUT_H);
						output = (BUF[3]<<8)|BUF[2];
					//	output = output * 2 / 32768; 						   //读取计算Y轴数据
						break;
				}
				case 3:
				{
						BUF[4] = MPU9250_ReadOneByte(ACCEL_ADDRESS,ACCEL_ZOUT_L);
						BUF[5] = MPU9250_ReadOneByte(ACCEL_ADDRESS,ACCEL_ZOUT_H);
						output = (BUF[5]<<8)|BUF[4];
						//output = output * 2 / 32768; 					       //读取计算Z轴数据
				}
				default:
						break;
		}
		return output;
}

uint16_t READ_MPU9250_GYRO(uint8_t axis)
{
		unsigned char BUF[10];
		uint16_t output;
	
		switch(axis)
		{
				case 1:
				{
						BUF[0]=MPU9250_ReadOneByte(GYRO_ADDRESS,GYRO_XOUT_L); 
						BUF[1]=MPU9250_ReadOneByte(GYRO_ADDRESS,GYRO_XOUT_H);
						output = (BUF[1]<<8)|BUF[0];
						output /= 16.4; 						   //读取计算X轴数据
						break;
				}
				case 2:
				{
						BUF[2]=MPU9250_ReadOneByte(GYRO_ADDRESS,GYRO_YOUT_L);
						BUF[3]=MPU9250_ReadOneByte(GYRO_ADDRESS,GYRO_YOUT_H);
						output = (BUF[3]<<8)|BUF[2];
						output /= 16.4; 						   //读取计算Y轴数据
						break;
				}
				case 3:
				{
						BUF[4]=MPU9250_ReadOneByte(GYRO_ADDRESS,GYRO_ZOUT_L);
						BUF[5]=MPU9250_ReadOneByte(GYRO_ADDRESS,GYRO_ZOUT_H);
						output = (BUF[5]<<8)|BUF[4];
						output /= 16.4; 					       //读取计算Z轴数据
				}
				default:
						break;
		}
		return output;
}

/*
uint16_t READ_MPU9250_MAG(uint8_t axis)
{
		unsigned char BUF[10];
		uint16_t T_X,T_Y,T_Z;		 //X,Y,Z轴
	
		MPU9250_WriteOneByte(GYRO_ADDRESS,0x37,0x02);//turn on Bypass Mode 
		delay_ms(10);	
		MPU9250_WriteOneByte(MAG_ADDRESS,0x0A,0x01);
		delay_ms(10);	
		BUF[0]=MPU9250_ReadOneByte (MAG_ADDRESS,MAG_XOUT_L);
		BUF[1]=MPU9250_ReadOneByte (MAG_ADDRESS,MAG_XOUT_H);
		T_X=(BUF[1]<<8)|BUF[0];

		BUF[2]=MPU9250_ReadOneByte(MAG_ADDRESS,MAG_YOUT_L);
		BUF[3]=MPU9250_ReadOneByte(MAG_ADDRESS,MAG_YOUT_H);
		T_Y=	(BUF[3]<<8)|BUF[2];
								 //读取计算Y轴数据

		BUF[4]=MPU9250_ReadOneByte(MAG_ADDRESS,MAG_ZOUT_L);
		BUF[5]=MPU9250_ReadOneByte(MAG_ADDRESS,MAG_ZOUT_H);
		T_Z=	(BUF[5]<<8)|BUF[4];
								 //读取计算Z轴数据
}*/


/************************************第2个MPU9250***************************************/

//初始化MPU9250，根据需要请参考pdf进行修改************************
void MPU9250_2_Init(void)
{
//		u8 data = 0;
/*
   Single_Write(GYRO_ADDRESS,PWR_M, 0x80);   //
   Single_Write(GYRO_ADDRESS,SMPL, 0x07);    //
   Single_Write(GYRO_ADDRESS,DLPF, 0x1E);    //±2000°
   Single_Write(GYRO_ADDRESS,INT_C, 0x00 );  //
   Single_Write(GYRO_ADDRESS,PWR_M, 0x00);   //
*/
//		MPU9250_2_WriteOneByte(GYRO_ADDRESS,PWR_MGMT_1, 0x00);	//解除休眠状态
//		MPU9250_2_WriteOneByte(GYRO_ADDRESS,SMPLRT_DIV, 0x07);
//		MPU9250_2_WriteOneByte(GYRO_ADDRESS,CONFIG, 0x06);
//		MPU9250_2_WriteOneByte(GYRO_ADDRESS,GYRO_CONFIG, 0x10);
//		MPU9250_2_WriteOneByte(GYRO_ADDRESS,ACCEL_CONFIG, 0x01);
		//MPU9250_2_WriteOneByte(GYRO_ADDRESS,ACCEL_CONFIG2, 0x08);
	
		MPU9250_2_WriteOneByte(GYRO_ADDRESS,PWR_MGMT_1, 0x80);	//RESET
		delay_ms(100);
		MPU9250_2_WriteOneByte(GYRO_ADDRESS,PWR_MGMT_1, 0x03);	//CLOCK_PLL
		delay_ms(1);
		MPU9250_2_WriteOneByte(GYRO_ADDRESS,PWR_MGMT_2, XYZ_GYRO & XYZ_ACCEL);	//Set Sensors
		delay_ms(1);
		MPU9250_2_WriteOneByte(GYRO_ADDRESS,SMPLRT_DIV, 0x00); //SAMPLE_RATE = Internal_Sample_Rate / (1 + SMPLRT_DIV)
		delay_ms(1);
		MPU9250_2_WriteOneByte(GYRO_ADDRESS,GYRO_CONFIG, 0x18);//MPU9250_2 Set Full Scale Gyro Range;Fchoice_b[1:0] = [00] enable DLPF
		delay_ms(1);
		MPU9250_2_WriteOneByte(GYRO_ADDRESS,ACCEL_CONFIG, 0x00);//MPU9250_2 Set Full Scale Accel Range PS:2G
		delay_ms(1);
		MPU9250_2_WriteOneByte(GYRO_ADDRESS,ACCEL_CONFIG2, 0x08);//MPU9250_2 Set Accel DLPF
		delay_ms(1);
		MPU9250_2_WriteOneByte(GYRO_ADDRESS,CONFIG, 0x03);//MPU9250_2 Set Gyro DLPF
		delay_ms(1);
		
	
//		if(MPU9250_2_Check())
//		{
//			GPIO_ResetBits(GPIOB, GPIO_Pin_9);
//		}
//		else
//		{
//			GPIO_SetBits(GPIOB, GPIO_Pin_9);
//		}
  //----------------
//	Single_Write(GYRO_ADDRESS,0x6A,0x00);//close Master Mode	

}
	

uint8_t MPU9250_2_Check(void) 
{
		uint8_t temp = 0;
		temp = MPU9250_2_ReadOneByte(GYRO_ADDRESS, WHO_AM_I);
   	if(WHO_AM_I_VAL == temp)  
   	{
   		return 1;
   	}
   	else 
   	{
   		return 0;
   	}	
}


//在指定地址读出一个数据
//ReadAddr:开始读数的地址  
//返回值  :读到的数据
uint8_t MPU9250_2_ReadOneByte(unsigned char SlaveAddress, unsigned char REG_Address)
{				  
		u8 temp=0;
    IIC2_Start();
		IIC2_Send_Byte(SlaveAddress);	 //发送设备地址
		IIC2_Wait_Ack();
		IIC2_Send_Byte(REG_Address);   //发送低起始地址
		IIC2_Wait_Ack();
		IIC2_Start();   
		IIC2_Send_Byte(SlaveAddress + 1);  	   
		IIC2_Wait_Ack();
    temp = IIC2_Read_Byte(0);		   
    IIC2_Stop();//产生一个停止条件	    
		return temp;
}

//在指定地址写入一个数据
//WriteAddr  :写入数据的目的地址    
//DataToWrite:要写入的数据
void MPU9250_2_WriteOneByte(unsigned char SlaveAddress, unsigned char REG_Address, unsigned char REG_data)
{				   	  	    																 
    IIC2_Start();  
		IIC2_Send_Byte(SlaveAddress);	   
		IIC2_Wait_Ack();
		IIC2_Send_Byte(REG_Address); 
		IIC2_Wait_Ack();	   
    IIC2_Send_Byte(REG_data);
		IIC2_Wait_Ack(); 	 										  		   		    	   
    IIC2_Stop();//产生一个停止条件 
		//delay_ms(2);	 
}	




//******读取MPU9250_2数据****************************************
int16_t READ_MPU9250_2_ACCEL(uint8_t axis)
{ 
		unsigned char BUF[10];
		uint16_t output;

		switch(axis)
		{
				case 1:
				{
						BUF[0] = MPU9250_2_ReadOneByte(ACCEL_ADDRESS,ACCEL_XOUT_L); 
						BUF[1] = MPU9250_2_ReadOneByte(ACCEL_ADDRESS,ACCEL_XOUT_H);
						output = (BUF[1]<<8)|BUF[0];
						//output = output * 2 / 32768; 						   //读取计算X轴数据
						break;
				}
				case 2:
				{
						BUF[2] = MPU9250_2_ReadOneByte(ACCEL_ADDRESS,ACCEL_YOUT_L);
						BUF[3] = MPU9250_2_ReadOneByte(ACCEL_ADDRESS,ACCEL_YOUT_H);
						output = (BUF[3]<<8)|BUF[2];
					//	output = output * 2 / 32768; 						   //读取计算Y轴数据
						break;
				}
				case 3:
				{
						BUF[4] = MPU9250_2_ReadOneByte(ACCEL_ADDRESS,ACCEL_ZOUT_L);
						BUF[5] = MPU9250_2_ReadOneByte(ACCEL_ADDRESS,ACCEL_ZOUT_H);
						output = (BUF[5]<<8)|BUF[4];
						//output = output * 2 / 32768; 					       //读取计算Z轴数据
				}
				default:
						break;
		}
		return output;
}

uint16_t READ_MPU9250_2_GYRO(uint8_t axis)
{
		unsigned char BUF[10];
		uint16_t output;
	
		switch(axis)
		{
				case 1:
				{
						BUF[0]=MPU9250_2_ReadOneByte(GYRO_ADDRESS,GYRO_XOUT_L); 
						BUF[1]=MPU9250_2_ReadOneByte(GYRO_ADDRESS,GYRO_XOUT_H);
						output = (BUF[1]<<8)|BUF[0];
						output /= 16.4; 						   //读取计算X轴数据
						break;
				}
				case 2:
				{
						BUF[2]=MPU9250_2_ReadOneByte(GYRO_ADDRESS,GYRO_YOUT_L);
						BUF[3]=MPU9250_2_ReadOneByte(GYRO_ADDRESS,GYRO_YOUT_H);
						output = (BUF[3]<<8)|BUF[2];
						output /= 16.4; 						   //读取计算Y轴数据
						break;
				}
				case 3:
				{
						BUF[4]=MPU9250_2_ReadOneByte(GYRO_ADDRESS,GYRO_ZOUT_L);
						BUF[5]=MPU9250_2_ReadOneByte(GYRO_ADDRESS,GYRO_ZOUT_H);
						output = (BUF[5]<<8)|BUF[4];
						output /= 16.4; 					       //读取计算Z轴数据
				}
				default:
						break;
		}
		return output;
}



/************************************第3个MPU9250***************************************/



//初始化MPU9250_3，根据需要请参考pdf进行修改************************
void MPU9250_3_Init(void)
{
//		u8 data = 0;
/*
   Single_Write(GYRO_ADDRESS,PWR_M, 0x80);   //
   Single_Write(GYRO_ADDRESS,SMPL, 0x07);    //
   Single_Write(GYRO_ADDRESS,DLPF, 0x1E);    //±2000°
   Single_Write(GYRO_ADDRESS,INT_C, 0x00 );  //
   Single_Write(GYRO_ADDRESS,PWR_M, 0x00);   //
*/
//		MPU9250_3_WriteOneByte(GYRO_ADDRESS,PWR_MGMT_1, 0x00);	//解除休眠状态
//		MPU9250_3_WriteOneByte(GYRO_ADDRESS,SMPLRT_DIV, 0x07);
//		MPU9250_3_WriteOneByte(GYRO_ADDRESS,CONFIG, 0x06);
//		MPU9250_3_WriteOneByte(GYRO_ADDRESS,GYRO_CONFIG, 0x10);
//		MPU9250_3_WriteOneByte(GYRO_ADDRESS,ACCEL_CONFIG, 0x01);
		//MPU9250_3_WriteOneByte(GYRO_ADDRESS,ACCEL_CONFIG2, 0x08);
	
		MPU9250_3_WriteOneByte(GYRO_ADDRESS,PWR_MGMT_1, 0x80);	//RESET
		delay_ms(100);
		MPU9250_3_WriteOneByte(GYRO_ADDRESS,PWR_MGMT_1, 0x03);	//CLOCK_PLL
		delay_ms(1);
		MPU9250_3_WriteOneByte(GYRO_ADDRESS,PWR_MGMT_2, XYZ_GYRO & XYZ_ACCEL);	//Set Sensors
		delay_ms(1);
		MPU9250_3_WriteOneByte(GYRO_ADDRESS,SMPLRT_DIV, 0x00); //SAMPLE_RATE = Internal_Sample_Rate / (1 + SMPLRT_DIV)
		delay_ms(1);
		MPU9250_3_WriteOneByte(GYRO_ADDRESS,GYRO_CONFIG, 0x18);//MPU9250_3 Set Full Scale Gyro Range;Fchoice_b[1:0] = [00] enable DLPF
		delay_ms(1);
		MPU9250_3_WriteOneByte(GYRO_ADDRESS,ACCEL_CONFIG, 0x00);//MPU9250_3 Set Full Scale Accel Range PS:2G
		delay_ms(1);
		MPU9250_3_WriteOneByte(GYRO_ADDRESS,ACCEL_CONFIG2, 0x08);//MPU9250_3 Set Accel DLPF
		delay_ms(1);
		MPU9250_3_WriteOneByte(GYRO_ADDRESS,CONFIG, 0x03);//MPU9250_3 Set Gyro DLPF
		delay_ms(1);
		
	
//		if(MPU9250_3_Check())
//		{
//			GPIO_ResetBits(GPIOB, GPIO_Pin_9);
//		}
//		else
//		{
//			GPIO_SetBits(GPIOB, GPIO_Pin_9);
//		}
  //----------------
//	Single_Write(GYRO_ADDRESS,0x6A,0x00);//close Master Mode	

}
	

uint8_t MPU9250_3_Check(void) 
{
		uint8_t temp = 0;
		temp = MPU9250_3_ReadOneByte(GYRO_ADDRESS, WHO_AM_I);
   	if(WHO_AM_I_VAL == temp)     	
		{
   		return 1;
   	}
   	else 
   	{
   		return 0;
   	}	
}


//在指定地址读出一个数据
//ReadAddr:开始读数的地址  
//返回值  :读到的数据
uint8_t MPU9250_3_ReadOneByte(unsigned char SlaveAddress, unsigned char REG_Address)
{				  
		u8 temp=0;
    IIC3_Start();
		IIC3_Send_Byte(SlaveAddress);	 //发送设备地址
		IIC3_Wait_Ack();
		IIC3_Send_Byte(REG_Address);   //发送低起始地址
		IIC3_Wait_Ack();
		IIC3_Start();   
		IIC3_Send_Byte(SlaveAddress + 1);  	   
		IIC3_Wait_Ack();
    temp = IIC3_Read_Byte(0);		   
    IIC3_Stop();//产生一个停止条件	    
		return temp;
}

//在指定地址写入一个数据
//WriteAddr  :写入数据的目的地址    
//DataToWrite:要写入的数据
void MPU9250_3_WriteOneByte(unsigned char SlaveAddress, unsigned char REG_Address, unsigned char REG_data)
{				   	  	    																 
    IIC3_Start();  
		IIC3_Send_Byte(SlaveAddress);	   
		IIC3_Wait_Ack();
		IIC3_Send_Byte(REG_Address); 
		IIC3_Wait_Ack();	   
    IIC3_Send_Byte(REG_data);
		IIC3_Wait_Ack(); 	 										  		   		    	   
    IIC3_Stop();//产生一个停止条件 
		//delay_ms(2);	 
}	




//******读取MPU9250_3数据****************************************
int16_t READ_MPU9250_3_ACCEL(uint8_t axis)
{ 
		unsigned char BUF[10];
		uint16_t output;

		switch(axis)
		{
				case 1:
				{
						BUF[0] = MPU9250_3_ReadOneByte(ACCEL_ADDRESS,ACCEL_XOUT_L); 
						BUF[1] = MPU9250_3_ReadOneByte(ACCEL_ADDRESS,ACCEL_XOUT_H);
						output = (BUF[1]<<8)|BUF[0];
						//output = output * 2 / 32768; 						   //读取计算X轴数据
						break;
				}
				case 2:
				{
						BUF[2] = MPU9250_3_ReadOneByte(ACCEL_ADDRESS,ACCEL_YOUT_L);
						BUF[3] = MPU9250_3_ReadOneByte(ACCEL_ADDRESS,ACCEL_YOUT_H);
						output = (BUF[3]<<8)|BUF[2];
					//	output = output * 2 / 32768; 						   //读取计算Y轴数据
						break;
				}
				case 3:
				{
						BUF[4] = MPU9250_3_ReadOneByte(ACCEL_ADDRESS,ACCEL_ZOUT_L);
						BUF[5] = MPU9250_3_ReadOneByte(ACCEL_ADDRESS,ACCEL_ZOUT_H);
						output = (BUF[5]<<8)|BUF[4];
						//output = output * 2 / 32768; 					       //读取计算Z轴数据
				}
				default:
						break;
		}
		return output;
}

uint16_t READ_MPU9250_3_GYRO(uint8_t axis)
{
		unsigned char BUF[10];
		uint16_t output;
	
		switch(axis)
		{
				case 1:
				{
						BUF[0]=MPU9250_3_ReadOneByte(GYRO_ADDRESS,GYRO_XOUT_L); 
						BUF[1]=MPU9250_3_ReadOneByte(GYRO_ADDRESS,GYRO_XOUT_H);
						output = (BUF[1]<<8)|BUF[0];
						output /= 16.4; 						   //读取计算X轴数据
						break;
				}
				case 2:
				{
						BUF[2]=MPU9250_3_ReadOneByte(GYRO_ADDRESS,GYRO_YOUT_L);
						BUF[3]=MPU9250_3_ReadOneByte(GYRO_ADDRESS,GYRO_YOUT_H);
						output = (BUF[3]<<8)|BUF[2];
						output /= 16.4; 						   //读取计算Y轴数据
						break;
				}
				case 3:
				{
						BUF[4]=MPU9250_3_ReadOneByte(GYRO_ADDRESS,GYRO_ZOUT_L);
						BUF[5]=MPU9250_3_ReadOneByte(GYRO_ADDRESS,GYRO_ZOUT_H);
						output = (BUF[5]<<8)|BUF[4];
						output /= 16.4; 					       //读取计算Z轴数据
				}
				default:
						break;
		}
		return output;
}
