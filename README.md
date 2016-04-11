# SysID\_Slave_MCU
---
##简介
该程序为硕士课题《分子液环式角加速度计的系统辨识与测试》中所用的下位机程序。其控制器为STM32F407VET6，开发环境为MDK5.11。

##目前实现的模块

- MCU自带AD模块，并开启DMA
- RTC模块
- 串行通信（包括printf函数）
- MTi惯导模块解算
- SPI接口的16位AD转换器AD7606
- sdio接口的tf卡模块，并开启DMA
- 移植fatfs([正点原子例程](http://www.openedv.com/))

##目前实现功能

- 读取角加速度数据（利用MCU自带AD或AD7606）
- 读取MTi惯导模块数据
- 存储数据为txt格式，并存入MicroSD卡，速率可达2kHz

##目前存在问题

- 开始存储数据后，有较高几率不响应按键，使得系统无法停止存储数据，或开始下一个数据文件的存储。

##之前存在问题解决方法

- 有较高几率存在所存数据文件为0kb的情况
	- 解决方案：参考该贴[FATFS周期往SD卡TXT里写ADC数据 占用资源问题](http://www.openedv.com/forum.php?mod=viewthread&tid=68901)。原因是外界因素会导致fatfs丢失数据，需要每隔一段时间调用f_sync以防止数据丢失。
