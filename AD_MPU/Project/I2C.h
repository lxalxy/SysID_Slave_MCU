#include "sys.h" 

//IO方向设置
#define SDA2_IN()  {GPIOB->MODER&=~(3<<(12*2));GPIOB->MODER|=0<<(12*2);}	//PB12输入模式
#define SDA2_OUT() {GPIOB->MODER&=~(3<<(12*2));GPIOB->MODER|=1<<(12*2);} //PB12输出模式
//IO操作函数	 
#define IIC_SCL2    PBout(11) //SCL
#define IIC_SDA2    PBout(12) //SDA	 
#define READ_SDA2   PBin(12)  //输入SDA 

////IO方向设置
#define SDA_IN()  {GPIOC->MODER&=~(3<<(2*2));GPIOC->MODER|=0<<(2*2);}	//PC2输入模式
#define SDA_OUT() {GPIOC->MODER&=~(3<<(2*2));GPIOC->MODER|=1<<(2*2);} //PC2输出模式
//IO操作函数	 
#define IIC_SCL    PCout(1) //SCL2
#define IIC_SDA    PCout(2) //SDA2	 
#define READ_SDA   PCin(2)  //输入SDA2 

//IO方向设置
#define SDA3_IN()  {GPIOC->MODER&=~(3<<(4*2));GPIOC->MODER|=0<<(4*2);}	//PC4输入模式
#define SDA3_OUT() {GPIOC->MODER&=~(3<<(4*2));GPIOC->MODER|=1<<(4*2);} //PC4输出模式
//IO操作函数	 
#define IIC_SCL3    PCout(3) //SCL3
#define IIC_SDA3    PCout(4) //SDA3	 
#define READ_SDA3   PCin(4)  //输入SDA3 


/*******************第1个I2C***********************/ 

//IIC所有操作函数
void IIC_Init(void);                //初始化IIC的IO口				 
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	  



/******************第2个I2C ***********************/

//IIC所有操作函数
void IIC2_Init(void);                //初始化IIC2的IO口				 
void IIC2_Start(void);				//发送IIC2开始信号
void IIC2_Stop(void);	  			//发送IIC2停止信号
void IIC2_Send_Byte(u8 txd);			//IIC2发送一个字节
u8 IIC2_Read_Byte(unsigned char ack);//IIC2读取一个字节
u8 IIC2_Wait_Ack(void); 				//IIC2等待ACK信号
void IIC2_Ack(void);					//IIC2发送ACK信号
void IIC2_NAck(void);				//IIC2不发送ACK信号

void IIC2_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC2_Read_One_Byte(u8 daddr,u8 addr);	 


/******************第3个I2C ************************/


//IIC所有操作函数
void IIC3_Init(void);                //初始化IIC3的IO口				 
void IIC3_Start(void);				//发送IIC3开始信号
void IIC3_Stop(void);	  			//发送IIC3停止信号
void IIC3_Send_Byte(u8 txd);			//IIC3发送一个字节
u8 IIC3_Read_Byte(unsigned char ack);//IIC3读取一个字节
u8 IIC3_Wait_Ack(void); 				//IIC3等待ACK信号
void IIC3_Ack(void);					//IIC3发送ACK信号
void IIC3_NAck(void);				//IIC3不发送ACK信号

void IIC3_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC3_Read_One_Byte(u8 daddr,u8 addr);	 
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
