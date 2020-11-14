#include <HAL_Device.h>
#include <hardware.h>

/* ����Ӳ����ԴΪ��ʼֵ */
static void hw_resource_reset(void)
{
	
	hw_interrupt_disable();
	__disable_fiq();
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, DISABLE);
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_ALL, DISABLE);
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_ALL, DISABLE);
}

/*
 * �����������пռ�, ��������ʱ��һ�����ص����пռ�, ����Ҫ��������
 * destaddr : Ŀ���ַ, �������еĵ�ַ
 * srcaddr	: Դ��ַ, ����洢���ĵ�ַ(����������洢��)
 * length	: �����ĳ��򳤶�
 */
void copy_to_runmem(unsigned long jmpaddr, unsigned long length)
{
	
}

/* 
 * ������ת�����пռ�
 * address : ���пռ�ĵ�ַ
 * ע�⣺�ڲ���Գ�����������е�ַ���в���
 */
void jmp_application(unsigned long address)
{
	hw_resource_reset();
	NVIC_SetVectorTable(address, 0x0);
	((void (*)(void))(*((unsigned long *)(address + 4))))();
}

/*
 * ����洢������
 * adderss : ��������ʼ��ַ
 * length  : �������ݵĳ���, ����������ݿ��Ը�����Ҫ�Զ���
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
 * д���ݵ��洢��
 * adderss : д��洢�ĵ�ַ
 * buffer  : д�������
 * length  : д������ݳ���
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
 * д���ݵ��洢��
 * adderss : ��ȡʱmemory��ʼ��ַ
 * buffer  : ��ȡ��������
 * length  : ��ȡ����󳤶�
 * ���ض�ȡ���ݵĳ���
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
 * ����У��CRC����
 * address : ����������
 * length  : �������ݵĳ���
 * ����CRC������
 */
unsigned long memory_crc32(unsigned long address, unsigned long length)
{
	unsigned long i;
	unsigned long crc;
	unsigned long data; 

	CRC_ResetDR();
	
	/* ����32BIT�����ֽ�CRC */
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

/* �洢���ʳ�ʼ�� */
void hardware_init(void)
{
	/* ��CRCʱ�� */ 
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
}
