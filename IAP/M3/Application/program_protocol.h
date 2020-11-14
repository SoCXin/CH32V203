#ifndef PROGRAM_PROTOCOL_H
#define PROGRAM_PROTOCOL_H

#include <define.h>

/* 定义协议处理缓冲区大小 */
#define PROTOCOL_RECV_BUFF_SIZE			1280
#define PROTOCOL_SEND_BUFF_SIZE			1280

/* ISP协议命令 */
#define PROGRAM_PROTOCOL_CMD_STATUS		0x01	// 改变编程状态
#define PROGRAM_PROTOCOL_CMD_READ_INFO	0x02	// 读取芯片信息
#define PROGRAM_PROTOCOL_CMD_ERASE		0x03	// 擦除Flash
#define PROGRAM_PROTOCOL_CMD_PROGRAM	0x04	// Flash编程
#define PROGRAM_PROTOCOL_CMD_READ		0x05	// 读取Flash数据
#define PROGRAM_PROTOCOL_CMD_VERIF		0x06	// 核实Flash数据	
#define PROGRAM_PROTOCOL_CMD_STARTUP	0x07	// 启动应用程序
#define PROGRAM_PROTOCOL_CMD_BOOTR		0x08	// 设置启动参数

/* 协议回调函数结构 */
struct program_protol_callback
{
	/* 读取MCU信息 */
	unsigned long (*read_infomation)(progam_flash_info_t infomaion);
	
	/* 擦除FLASH */
	unsigned long (*erase)(memory_range_t range);
	
	/* 编程 */
	unsigned long (*program)(memory_range_t range, unsigned char *buffer);
	
	/* 读取FLASH数据 */
	unsigned long (*read)(unsigned char *buffer, unsigned long max, memory_range_t range);
	
	/* 校验数据 */
	unsigned long (*verif)(memory_range_t range);
	
	/* 启动应用 */
	unsigned long (*startup_application)(void);
	
	/* 设置APP信息 */
	unsigned long (*bootarg_set)(const char *bootarg);
};

typedef struct program_protol_callback *program_protol_callback_t;

/* 编程状态描述 */
enum program_status
{
	PROGRAM_STATUS_STOP = 0,
	PROGRAM_STATUS_SATRT
};

/* 编程启动数据流状态 */
enum program_start_status
{
	PROGRAM_START_SREAM0 = 0,
	PROGRAM_START_SREAM1,
	PROGRAM_START_OK
};

#define PROGRAM_SATRT_DATA_STREAM0		"0123456789012345678901234567890123456789"
#define PROGRAM_SATRT_DATA_STREAM1		"9876543210987654321098765432109876543210"


/* 协议释放用户操作API */
void program_protocol_init(void);
void program_protocol_callback_config(const program_protol_callback_t callback);
void program_protocol_startup_address_set(unsigned long address);
void program_protocol_handler(void);

#endif
