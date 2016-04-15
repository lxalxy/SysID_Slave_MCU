#include "sys.h" 

//IO��������
#define SDA2_IN()  {GPIOB->MODER&=~(3<<(12*2));GPIOB->MODER|=0<<(12*2);}	//PB12����ģʽ
#define SDA2_OUT() {GPIOB->MODER&=~(3<<(12*2));GPIOB->MODER|=1<<(12*2);} //PB12���ģʽ
//IO��������	 
#define IIC_SCL2    PBout(11) //SCL
#define IIC_SDA2    PBout(12) //SDA	 
#define READ_SDA2   PBin(12)  //����SDA 

////IO��������
#define SDA_IN()  {GPIOC->MODER&=~(3<<(2*2));GPIOC->MODER|=0<<(2*2);}	//PC2����ģʽ
#define SDA_OUT() {GPIOC->MODER&=~(3<<(2*2));GPIOC->MODER|=1<<(2*2);} //PC2���ģʽ
//IO��������	 
#define IIC_SCL    PCout(1) //SCL2
#define IIC_SDA    PCout(2) //SDA2	 
#define READ_SDA   PCin(2)  //����SDA2 

//IO��������
#define SDA3_IN()  {GPIOC->MODER&=~(3<<(4*2));GPIOC->MODER|=0<<(4*2);}	//PC4����ģʽ
#define SDA3_OUT() {GPIOC->MODER&=~(3<<(4*2));GPIOC->MODER|=1<<(4*2);} //PC4���ģʽ
//IO��������	 
#define IIC_SCL3    PCout(3) //SCL3
#define IIC_SDA3    PCout(4) //SDA3	 
#define READ_SDA3   PCin(4)  //����SDA3 


/*******************��1��I2C***********************/ 

//IIC���в�������
void IIC_Init(void);                //��ʼ��IIC��IO��				 
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	  



/******************��2��I2C ***********************/

//IIC���в�������
void IIC2_Init(void);                //��ʼ��IIC2��IO��				 
void IIC2_Start(void);				//����IIC2��ʼ�ź�
void IIC2_Stop(void);	  			//����IIC2ֹͣ�ź�
void IIC2_Send_Byte(u8 txd);			//IIC2����һ���ֽ�
u8 IIC2_Read_Byte(unsigned char ack);//IIC2��ȡһ���ֽ�
u8 IIC2_Wait_Ack(void); 				//IIC2�ȴ�ACK�ź�
void IIC2_Ack(void);					//IIC2����ACK�ź�
void IIC2_NAck(void);				//IIC2������ACK�ź�

void IIC2_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC2_Read_One_Byte(u8 daddr,u8 addr);	 


/******************��3��I2C ************************/


//IIC���в�������
void IIC3_Init(void);                //��ʼ��IIC3��IO��				 
void IIC3_Start(void);				//����IIC3��ʼ�ź�
void IIC3_Stop(void);	  			//����IIC3ֹͣ�ź�
void IIC3_Send_Byte(u8 txd);			//IIC3����һ���ֽ�
u8 IIC3_Read_Byte(unsigned char ack);//IIC3��ȡһ���ֽ�
u8 IIC3_Wait_Ack(void); 				//IIC3�ȴ�ACK�ź�
void IIC3_Ack(void);					//IIC3����ACK�ź�
void IIC3_NAck(void);				//IIC3������ACK�ź�

void IIC3_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC3_Read_One_Byte(u8 daddr,u8 addr);	 
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
