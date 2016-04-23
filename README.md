# SysID\_Slave_MCU

##简介
该程序为硕士课题《分子液环式角加速度计的系统辨识与测试》中所用的下位机程序之一。用于利用多加速度计解算角加速度。与之前程序不同的是，该程序参考st官方固件库对sdio驱动进行重新实现，包括DMA和缓存区设计。其控制器为STM32F407VET6，开发环境为MDK5.11。

##目前实现的模块

- 串行通信（包括printf函数）
- SPI接口的16位AD转换器AD7606
- sdio接口的tf卡模块，并开启DMA
- 移植fatfs
- 缓存区循环队列设计[参考例程](http://www.cnblogs.com/hiker-blogs/p/3694567.html)
- I2C接口的MPU9250驱动

##目前实现功能

- 定时读取角加速度数据（利用AD7606）及3个MPU9250加速度计数据
- 将读取的数据存入缓存区中
- while(1)中将缓存区中数据存入MicroSD卡

##目前存在问题

- 250Hz及以上的定时器频率会导致存储数据混乱，大概连续存100个数据包后出现问题，一段时候后恢复，以此循环

##之前存在问题解决方法

- MPU9250读取WHO_AM_I存在问题
	- 解决方案：模拟I2C中时序存在问题，延时过短，导致I2C读取不稳定。

##参考文献

1. Padgaonkar A J, Krieger K W, King A I. Measurement of angular acceleration of a rigid body using linear accelerometers[J]. Journal of Applied Mechanics, 1975, 42(3): 552-556.
2. 熊永虎, 马宝华, 彭兴平. 用线加速度计测量角加速度和线加速度 (英文)[J]. 北京理工大学学报: 英文版, 2000 (3): 307-311.
