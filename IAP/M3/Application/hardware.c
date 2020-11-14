#include <HAL_Device.h>
#include <hardware.h>

/* 设置硬件资源为初始值 */
static void hw_resource_reset(void)
{
	
	hw_interrupt_disable();
	__disable_fiq();
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, DISABLE);
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_ALL, DISABLE);
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_ALL, DISABLE);
}

/*
 * 拷贝程序到运行空间, 程序下载时不一定下载到运行空间, 所以要拷贝数据
 * destaddr : 目标地址, 程序运行的地址
 * srcaddr	: 源地址, 程序存储器的地址(可以是任意存储器)
 * length	: 拷贝的程序长度
 */
void copy_to_runmem(unsigned long jmpaddr, unsigned long length)
{
	
}

/* 
 * 程序跳转至运行空间
 * address : 运行空间的地址
 * 注意：内部需对程序的真正运行地址进行操作
 */
void jmp_application(unsigned long address)
{
	hw_resource_reset();
	NVIC_SetVectorTable(address, 0x0);
	((void (*)(void))(*((unsigned long *)(address + 4))))();
}

/*
 * 程序存储器擦除
 * adderss : 擦除的起始地址
 * length  : 擦除数据的长度, 擦除后的数据可以根据需要自定义
 */
void memory_erase(unsigned long adderss, unsigned long length)
{
	unsigned char page_max;
	
	page_max = length / 1024;
	if(length & 0x3F) page_max++;
	FLASH_Unlock();
	while(page_max--)
	{
		FLASH_ErasePage(adderss);
		adderss += 1024;
	}
	FLASH_Lock();
}

/*
 * 写数据到存储器
 * adderss : 写入存储的地址
 * buffer  : 写入的数据
 * length  : 写入的数据长度
 */
void memory_write(unsigned long adderss, unsigned char *buffer, unsigned long length)
{
	unsigned long i;
	
	FLASH_Unlock();
	for(i = 0; i < length; i += 4)
	{
			FLASH_ProgramWord(adderss, *((unsigned long *)buffer));
			adderss += 4;
			buffer += 4;
	}
	FLASH_Lock();
}

/*
 * 写数据到存储器
 * adderss : 读取时memory起始地址
 * buffer  : 读取到的数据
 * length  : 读取的最大长度
 * 返回读取数据的长度
 */
unsigned long memory_read(unsigned long adderss, unsigned char *buffer, unsigned long length)
{
	unsigned long i;
	
	for(i = 0; i < length; i += 4)
	{
		*((unsigned long *)buffer) = *((unsigned long *)adderss);
		adderss += 4;
		buffer += 4;
	}
	
	return length;
}

/*
 * 程序校验CRC计算
 * address : 计算数据流
 * length  : 计算数据的长度
 * 返回CRC计算结果
 */
unsigned long memory_crc32(unsigned long address, unsigned long length)
{
	unsigned long i;
	unsigned long crc;
	unsigned long data; 

	CRC_ResetDR();
	
	/* 计算32BIT整数字节CRC */
	for(i = 0; i < (length & (~0x3)); i++)
	{
		data = *(unsigned long *)address;
		address += 4;
		data = (data >> 16) | (data << 16);
		data = ((data & 0xFF00FF00) >> 8) | ((data & 0x00FF00FF) << 8);
		CRC_CalcCRC(data);
	}
	
	length &= 3;
	if(length)
	{
		data = *(unsigned long *)address;
		address += 4;
		data = (data >> 16) | (data << 16);
		data = ((data & 0xFF00FF00) >> 8) | ((data & 0x00FF00FF) << 8);
		
		if(length > 2) data &= 0xFFFFFF00;
		else if(length > 1) data &= 0xFFFF0000;
		else data &= 0xFF000000;
		
		crc = CRC_CalcBlockCRC((uint32_t *)&data, 1);
	}
	
	return crc;
}

/* 存储介质初始化 */
void hardware_init(void)
{
	/* 打开CRC时钟 */ 
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
}
