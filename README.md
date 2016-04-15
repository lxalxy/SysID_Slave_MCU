# SysID\_Slave_MCU

##简介
该程序为硕士课题《分子液环式角加速度计的系统辨识与测试》中所用的下位机程序之一。用于利用多加速度计解算角加速度。其控制器为STM32F407VET6，开发环境为MDK5.11。

##目前实现的模块

- 串行通信（包括printf函数）
- SPI接口的16位AD转换器AD7606

- sdio接口的tf卡模块，并开启DMA
- 移植fatfs([正点原子例程](http://www.openedv.com/))
- I2C接口的MPU9250驱动

##目前实现功能

- 读取角加速度数据（利用AD7606）
- 读取3个MPU9250加速度计数据
- 存储数据为txt格式，并存入MicroSD卡

##目前存在问题

- 利用定时器读取MPU9250数据时，如果定时频率小于10Hz，可以正常读取；档定时频率大于10Hz，数据开始混乱。

##之前存在问题解决方法

- MPU9250读取WHO_AM_I存在问题
	- 解决方案：模拟I2C中时序存在问题，延时过短，导致I2C读取不稳定。

##参考文献

1. Padgaonkar A J, Krieger K W, King A I. Measurement of angular acceleration of a rigid body using linear accelerometers[J]. Journal of Applied Mechanics, 1975, 42(3): 552-556.
2. 熊永虎, 马宝华, 彭兴平. 用线加速度计测量角加速度和线加速度 (英文)[J]. 北京理工大学学报: 英文版, 2000 (3): 307-311.
