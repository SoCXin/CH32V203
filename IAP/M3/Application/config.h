#ifndef _CONFIG_H
#define _CONFIG_H

/* 
 * ������ͺͰ汾�� 
 * ISP�������(BIT31..BIT24) 
 * APP���汾��(BIT23..BIT16) 
 * APP�Ӱ汾��(BIT15..BIT08) 
 * APPС�汾��(BIT07..BIT00) 
 */
#define ISAP_IAP_VIRSION_TYPE	0ul			// ISP�������
#define ISAP_IAP_VIRSION_MAIN	1ul			// APP���汾��
#define ISAP_IAP_VIRSION_SUB	0ul			// APP�Ӱ汾��
#define ISAP_IAP_VIRSION_SMALL	0ul			// APPС�汾��
#define ISAP_IAP_VIRSION \
	((ISAP_IAP_VIRSION_TYPE << 24) | (ISAP_IAP_VIRSION_MAIN << 16) | (ISAP_IAP_VIRSION_SUB << 8) | ISAP_IAP_VIRSION_SMALL)	
	 
/* 
 * ���嵥Ƭ�ͺ�
 * оƬ����(BIT31..BIT16) 
 * оƬ����(BIT15..BIT08) 
 *оƬ����(BIT07..BIT00) 
 */
#define MCU_MANUFACTURER		0ul			// оƬ����
#define MCU_CLASS_MAIN			0ul			// оƬ����
#define MCU_CLASS_SUB			0ul			// оƬ����
#define MCU_CLASS_INFOMATION \
	((MCU_MANUFACTURER << 16) | (MCU_CLASS_MAIN << 8) | (MCU_CLASS_SUB))
	
/*
 * ˵�����򱻱�̵�NOR���ʹ洢��������������
 * NOR���ʹ洢��������ֱ������, �����洢��
 * �������Ҫ�ٴμ��ص�NOR���ʹ洢���в�������
 * ���ط�ΪFLASH��RAM, RAMÿ������������Ҫ���¼���
 * Flash���͵�ֻ����һ��, �Ժ󽫲������¼���
 */
#define PROGRAM_MEMORY_TYPE_NOR		1

/* APP�������еĵ�ַ */
#define APPLICATION_ADDRESS			0x08002000

#endif
