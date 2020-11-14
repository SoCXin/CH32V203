#ifndef PROGRAM_PROTOCOL_H
#define PROGRAM_PROTOCOL_H

#include <define.h>

/* ����Э�鴦��������С */
#define PROTOCOL_RECV_BUFF_SIZE			1280
#define PROTOCOL_SEND_BUFF_SIZE			1280

/* ISPЭ������ */
#define PROGRAM_PROTOCOL_CMD_STATUS		0x01	// �ı���״̬
#define PROGRAM_PROTOCOL_CMD_READ_INFO	0x02	// ��ȡоƬ��Ϣ
#define PROGRAM_PROTOCOL_CMD_ERASE		0x03	// ����Flash
#define PROGRAM_PROTOCOL_CMD_PROGRAM	0x04	// Flash���
#define PROGRAM_PROTOCOL_CMD_READ		0x05	// ��ȡFlash����
#define PROGRAM_PROTOCOL_CMD_VERIF		0x06	// ��ʵFlash����	
#define PROGRAM_PROTOCOL_CMD_STARTUP	0x07	// ����Ӧ�ó���
#define PROGRAM_PROTOCOL_CMD_BOOTR		0x08	// ������������

/* Э��ص������ṹ */
struct program_protol_callback
{
	/* ��ȡMCU��Ϣ */
	unsigned long (*read_infomation)(progam_flash_info_t infomaion);
	
	/* ����FLASH */
	unsigned long (*erase)(memory_range_t range);
	
	/* ��� */
	unsigned long (*program)(memory_range_t range, unsigned char *buffer);
	
	/* ��ȡFLASH���� */
	unsigned long (*read)(unsigned char *buffer, unsigned long max, memory_range_t range);
	
	/* У������ */
	unsigned long (*verif)(memory_range_t range);
	
	/* ����Ӧ�� */
	unsigned long (*startup_application)(void);
	
	/* ����APP��Ϣ */
	unsigned long (*bootarg_set)(const char *bootarg);
};

typedef struct program_protol_callback *program_protol_callback_t;

/* ���״̬���� */
enum program_status
{
	PROGRAM_STATUS_STOP = 0,
	PROGRAM_STATUS_SATRT
};

/* �������������״̬ */
enum program_start_status
{
	PROGRAM_START_SREAM0 = 0,
	PROGRAM_START_SREAM1,
	PROGRAM_START_OK
};

#define PROGRAM_SATRT_DATA_STREAM0		"0123456789012345678901234567890123456789"
#define PROGRAM_SATRT_DATA_STREAM1		"9876543210987654321098765432109876543210"


/* Э���ͷ��û�����API */
void program_protocol_init(void);
void program_protocol_callback_config(const program_protol_callback_t callback);
void program_protocol_startup_address_set(unsigned long address);
void program_protocol_handler(void);

#endif
