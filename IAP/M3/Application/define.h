#ifndef _DEFINE_H
#define _DEFINE_H

#include <string.h>
#include <config.h>

#define False					0
#define True					(!False)

#ifndef NULL
#define NULL					((void*)0)
#endif

#define PROGRAM_EOK				0
#define PROGRAM_ERROR			1

#define ARRAY_SIZE(arr)			(sizeof(arr) / sizeof(arr[0]))
#define GET_BYTE(data, byte)	(unsigned char)(data >> (byte * 8))

/* ���Flash����Χ�ṹ */
struct memory_range
{
	unsigned long address;					// �ɱ��Flash��ʼ��ַ
	unsigned long length;					// �ɱ�̵�Flash�ֽ���
};

typedef struct memory_range *memory_range_t;


/* �ɱ��Flash��Ϣ���� */	
struct progam_flash_info
{
	unsigned char area_num;					// �ɱ������ĸ���
	memory_range_t range_list;				// �ɱ�������
};

typedef struct progam_flash_info *progam_flash_info_t;

/* APP��Ϣ���� */
struct application_info
{
	char *app_name;					// APP���ֳ���
	char *auth_name;				// APP������
	unsigned long version;			// �汾��
	unsigned long start;			// APP��ʼ��ַ
	unsigned long length;			// APP���ݴ�С
};

typedef struct application_info *application_info_t;

#endif
