#include "sys.h" 


// 定义MPU9250内部地址
//****************************************
#define SELF_TEST_X_GYRO        0x00
#define SELF_TEST_Y_GYRO        0x01
#define SELF_TEST_Z_GYRO        0x02

#define SELF_TEST_X_ACCEL       0x0D
#define SELF_TEST_Y_ACCEL       0x0E
#define SELF_TEST_Z_ACCEL       0x0F

#define XG_OFFSET_H             0x13
#define XG_OFFSET_L             0x14
#define YG_OFFSET_H             0x15
#define YG_OFFSET_L             0x16
#define ZG_OFFSET_H             0x17
#define ZG_OFFSET_L             0x18

#define	SMPLRT_DIV		0x19	//陀螺仪采样率，典型值：0x07(125Hz)
#define	CONFIG			0x1A	//低通滤波频率，典型值：0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define	ACCEL_CONFIG	0x1C	//加速计自检、测量范围：0x00(不自检，2G)
#define	ACCEL_CONFIG2	0x1D	//加速度计频率

#define LP_ACCEL_ODR            0x1E
#define WOM_THR                 0x1F

#define FIFO_EN                 0x23
#define I2C_MST_CTRL            0x24
#define I2C_SLV0_ADDR           0x25
#define I2C_SLV0_REG            0x26
#define I2C_SLV0_CTRL           0x27
#define I2C_SLV1_ADDR           0x28
#define I2C_SLV1_REG            0x29
#define I2C_SLV1_CTRL           0x2A
#define I2C_SLV2_ADDR           0x2B
#define I2C_SLV2_REG            0x2C
#define I2C_SLV2_CTRL           0x2D
#define I2C_SLV3_ADDR           0x2E
#define I2C_SLV3_REG            0x2F
#define I2C_SLV3_CTRL           0x30
#define I2C_SLV4_ADDR           0x31
#define I2C_SLV4_REG            0x32
#define I2C_SLV4_DO             0x33
#define I2C_SLV4_CTRL           0x34
#define I2C_SLV4_DI             0x35
#define I2C_MST_STATUS          0x36
#define INT_PIN_CFG             0x37
#define INT_ENABLE              0x38

#define INT_STATUS              0x3A

#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40

#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42

#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48

		
#define MAG_XOUT_L		0x03
#define MAG_XOUT_H		0x04
#define MAG_YOUT_L		0x05
#define MAG_YOUT_H		0x06
#define MAG_ZOUT_L		0x07
#define MAG_ZOUT_H		0x08


#define I2C_SLV0_DO             0x63
#define I2C_SLV1_DO             0x64
#define I2C_SLV2_DO             0x65
#define I2C_SLV3_DO             0x66
#define I2C_MST_DELAY_CTRL      0x67
#define SIGNAL_PATH_RESET       0x68
#define MOT_DETECT_CTRL         0x69
#define USER_CTRL               0x6A
#define PWR_MGMT_1              0x6B  //电源管理，典型值：0x00(正常启用)
#define PWR_MGMT_2              0x6C

#define FIFO_COUNTH             0x72
#define FIFO_COUNTL             0x73
#define FIFO_R_W                0x74
#define WHO_AM_I								0x75
#define XA_OFFSET_H             0x77
#define XA_OFFSET_L             0x78

#define YA_OFFSET_H             0x7A
#define YA_OFFSET_L             0x7B

#define ZA_OFFSET_H             0x7D
#define ZA_OFFSET_L             0x7E
//
#define I2C_READ 0x80




//****************************

#define WHO_AM_I_VAL  0x71 //identity of MPU9250 is 0x71. identity of MPU9255 is 0x73.
#define XYZ_GYRO 0xC7
#define XYZ_ACCEL 0xF8



//****************************

#define	GYRO_ADDRESS   0xD0	  //陀螺地址
#define MAG_ADDRESS    0x18   //磁场地址
#define ACCEL_ADDRESS  0xD0 


uint8_t MPU9250_ReadOneByte(unsigned char SlaveAddress, unsigned char REG_Address);
void MPU9250_WriteOneByte(unsigned char SlaveAddress, unsigned char REG_Address, unsigned char REG_data);
void MPU9250_Init(void);
int16_t READ_MPU9250_ACCEL(uint8_t);
uint16_t READ_MPU9250_GYRO(uint8_t axis);
uint16_t READ_MPU9250_MAG(uint8_t);
uint8_t MPU9250_Check(void);


uint8_t MPU9250_2_ReadOneByte(unsigned char SlaveAddress, unsigned char REG_Address);
void MPU9250_2_WriteOneByte(unsigned char SlaveAddress, unsigned char REG_Address, unsigned char REG_data);
void MPU9250_2_Init(void);
int16_t READ_MPU9250_2_ACCEL(uint8_t);
uint16_t READ_MPU9250_2_GYRO(uint8_t axis);
uint16_t READ_MPU9250_2_MAG(uint8_t);
uint8_t MPU9250_2_Check(void);


uint8_t MPU9250_3_ReadOneByte(unsigned char SlaveAddress, unsigned char REG_Address);
void MPU9250_3_WriteOneByte(unsigned char SlaveAddress, unsigned char REG_Address, unsigned char REG_data);
void MPU9250_3_Init(void);
int16_t READ_MPU9250_3_ACCEL(uint8_t);
uint16_t READ_MPU9250_3_GYRO(uint8_t axis);
uint16_t READ_MPU9250_3_MAG(uint8_t);
uint8_t MPU9250_3_Check(void);
