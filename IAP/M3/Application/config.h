#ifndef _CONFIG_H
#define _CONFIG_H

/* 
 * 软件类型和版本号 
 * ISP软件类型(BIT31..BIT24) 
 * APP主版本号(BIT23..BIT16) 
 * APP子版本号(BIT15..BIT08) 
 * APP小版本号(BIT07..BIT00) 
 */
#define ISAP_IAP_VIRSION_TYPE	0ul			// ISP软件类型
#define ISAP_IAP_VIRSION_MAIN	1ul			// APP主版本号
#define ISAP_IAP_VIRSION_SUB	0ul			// APP子版本号
#define ISAP_IAP_VIRSION_SMALL	0ul			// APP小版本号
#define ISAP_IAP_VIRSION \
	((ISAP_IAP_VIRSION_TYPE << 24) | (ISAP_IAP_VIRSION_MAIN << 16) | (ISAP_IAP_VIRSION_SUB << 8) | ISAP_IAP_VIRSION_SMALL)	
	 
/* 
 * 定义单片型号
 * 芯片厂商(BIT31..BIT16) 
 * 芯片主类(BIT15..BIT08) 
 *芯片子类(BIT07..BIT00) 
 */
#define MCU_MANUFACTURER		0ul			// 芯片厂商
#define MCU_CLASS_MAIN			0ul			// 芯片主类
#define MCU_CLASS_SUB			0ul			// 芯片子类
#define MCU_CLASS_INFOMATION \
	((MCU_MANUFACTURER << 16) | (MCU_CLASS_MAIN << 8) | (MCU_CLASS_SUB))
	
/*
 * 说明程序被编程到NOR类型存储器还是其他类型
 * NOR类型存储器编程完可直接运行, 其他存储器
 * 编程完需要再次加载到NOR类型存储器中才能运行
 * 加载分为FLASH和RAM, RAM每次启动机器都要重新加载
 * Flash类型的只加载一次, 以后将不再重新加载
 */
#define PROGRAM_MEMORY_TYPE_NOR		1

/* APP程序运行的地址 */
#define APPLICATION_ADDRESS			0x08002000

#endif
