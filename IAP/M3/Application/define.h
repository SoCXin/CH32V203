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

/* 编程Flash区域范围结构 */
struct memory_range
{
	unsigned long address;					// 可编程Flash起始地址
	unsigned long length;					// 可编程的Flash字节数
};

typedef struct memory_range *memory_range_t;


/* 可编程Flash信息描述 */	
struct progam_flash_info
{
	unsigned char area_num;					// 可编程区域的个数
	memory_range_t range_list;				// 可编程区域表
};

typedef struct progam_flash_info *progam_flash_info_t;

/* APP信息描述 */
struct application_info
{
	char *app_name;					// APP名字长度
	char *auth_name;				// APP著作者
	unsigned long version;			// 版本号
	unsigned long start;			// APP起始地址
	unsigned long length;			// APP内容大小
};

typedef struct application_info *application_info_t;

#endif
