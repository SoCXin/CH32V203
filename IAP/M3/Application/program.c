#include <define.h>
#include <soft_timer.h>
#include <program_protocol.h>
#include <hardware.h>

/* ISP IAP软件占用程序控件范围 */
const struct memory_range isp_iap_memory_range = 
{
	0x08000000, 0x2000
};

/* 可编程范围列表描述 */
const struct memory_range progam_flash_range_table[] = 
{
	{0x08002000, 0xE000},
};

/* 可编程信息描述 */
const struct progam_flash_info progam_flash_infomation = 
{
	ARRAY_SIZE(progam_flash_range_table),
	(memory_range_t)progam_flash_range_table
};

/* 获取芯片程序起始地址 */
unsigned long program_application_strtup_address(void)
{
	return(APPLICATION_ADDRESS);
}

/* 程序存储类型 */
static unsigned char program_memory_type;	

/* 检测数据范围是否在可使用范围内 */
unsigned char memory_available(memory_range_t range)
{
	struct memory_range check_range;
	unsigned long count;
	unsigned char i;
	
	memcpy(&check_range, range, sizeof(struct memory_range));
	
	for(i = 0; i < progam_flash_infomation.area_num; i++)
	{
		if(check_range.address >= progam_flash_infomation.range_list[i].address)
		{
			if((check_range.address + check_range.length) < (progam_flash_infomation.range_list[i].address + progam_flash_infomation.range_list[i].length))
			{
				return True;
			}
			else
			{
				count = progam_flash_infomation.range_list[i].length - (check_range.address - progam_flash_infomation.range_list[i].address);
				check_range.address += count;
				check_range.length -= count;
			}
		}
	}
	
	return False;
}

/* 读取芯片和厂商等信息回调函数 */
static unsigned long read_infomation_callback(progam_flash_info_t infomaion)
{
	unsigned long result = 0;
	
	memcpy(infomaion, &progam_flash_infomation, sizeof(struct progam_flash_info));
	
	return result;
}

/* 内存擦除回调函数 */
static unsigned long erase_callback(memory_range_t range)
{
	if(memory_available(range))
	{
		memory_erase(range->address, range->length);
		return PROGRAM_EOK;
	}
	
	return PROGRAM_ERROR;
}

/* 编程回调函数 */
static unsigned long program_callback(memory_range_t range, unsigned char *buffer)
{
	if(memory_available(range))
	{
		memory_write(range->address, buffer, range->length);
		return PROGRAM_EOK;
	}
	
	
	return PROGRAM_ERROR;
}

/* 读取Flash数据回调 */
static unsigned long read_callback(unsigned char *buffer, unsigned long max, memory_range_t range)
{
	unsigned long result = 0;
	
	if(memory_available(range))
	{
		result = (range->length > max)? max : range->length;
		memory_read(range->address, buffer, result);
	}
	
	return result;
}

/* 核实Memory数据 */
static unsigned long verif_callback(memory_range_t range)
{
	if(memory_available(range))
	{
		return memory_crc32(range->address, range->length);
	}
	
	return 0;
}

/* 启动应用 */
static unsigned long startup_application_callback(void)
{
	unsigned long result = 0;
	struct application_info info;

	if(program_memory_type != PROGRAM_MEMORY_TYPE_NOR)
	{
		/* 解析启动参数 */
		
		
		/* 运行空间不允许与ISP程序有交集 */
		if((info.start < isp_iap_memory_range.address) && (info.start >= (isp_iap_memory_range.address + isp_iap_memory_range.length)))
		{
			/* 拷贝程序到运行空间 */
			copy_to_runmem(info.start, info.length);
			
			/* 启动应用程序 */
			jmp_application(info.start);
		}
	}
	else
	{
		/* 直接写数据到运行空间则要指定启动地址启动应用程序(当前这个地址是固定的) */
		jmp_application(program_application_strtup_address());
	}
	
	return result;
}

///* 词法分析 */
//unsigned char lexical_analysis(char **next, char *word, unsigned char length)
//{
//	unsigned char count;
//	
//	count = 0;
//	*word = '\0';
//	
//	/* 跳过空格和制表符 */
//	while(((**next == ' ') || (**next == '\t')) && (**next != '\0')) (*next)++;
//	
//	/* 提取单词 */
//	while((**next != '\0') && (**next != ' ') && (length--))
//	{
//		*word++ = **next;
//		(*next)++;
//		count++;
//	}
//	
//	/* 可能缓冲区不够用, 单词没有被提取完整, 这个时候要跳过整个单词的位置 */
//	while((**next != ' ') || (**next != '\t') || (**next != '\0')) (*next)++;
//	*word = '\0';
//	
//	return count;
//}

///* 语法检测 */
//unsigned char bootarg_checking(const char *string)
//{
//	char *start;
//	char word[32];
//	
//	start = (char *)string;
//	lexical_analysis(&start, word, 32) == 0) return False;
//	if(lexical_analysis(&start, word, 32) != 1) return False;
//	if(word[0] != '=') return False;
//	if(lexical_analysis(&start, word, 32) == 0) return False;
//	lexical_analysis(&start, word, 32);
//	if(word[0] != ',' && word[0] != '\0') return False;
//	
//	return True;
//}

//unsigned long parser_args(const char *bootarg, char *argv[], unsigned long max)
//{
//	
//}

///* 解析启动参数 */
//void parser_application_info(const char *bootarg, application_info_t info)
//{
//	char *argv[20];
//	unsigned char argc;
//	unsigned char i;
//	
//	argc = parser_args(bootarg, argv, 20);
//	for(i = 0; i < argc; i++)
//	{
//		if()
//	}
//}

/* 设置启动信息 */
static unsigned long bootarg_set(const char *bootarg)
{
	return 0;
}

const struct program_protol_callback callback_operation = 
{
	.read_infomation = read_infomation_callback,
	.erase = erase_callback,
	.program = program_callback,
	.read = read_callback,
	.verif = verif_callback,
	.startup_application = startup_application_callback,
	.bootarg_set = bootarg_set
};

/* 初始化编程方法和参数 */
void program_init(void)
{
	/* 初始化存储器 */
	hardware_init();
	
	/* 设置操作函数 */
	program_protocol_callback_config((program_protol_callback_t)&callback_operation);
	program_memory_type = PROGRAM_MEMORY_TYPE_NOR;
}

