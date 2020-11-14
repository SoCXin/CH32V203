#include <define.h>
#include <soft_timer.h>
#include <program_protocol.h>
#include <hardware.h>

/* ISP IAP���ռ�ó���ؼ���Χ */
const struct memory_range isp_iap_memory_range = 
{
	0x08000000, 0x2000
};

/* �ɱ�̷�Χ�б����� */
const struct memory_range progam_flash_range_table[] = 
{
	{0x08002000, 0xE000},
};

/* �ɱ����Ϣ���� */
const struct progam_flash_info progam_flash_infomation = 
{
	ARRAY_SIZE(progam_flash_range_table),
	(memory_range_t)progam_flash_range_table
};

/* ��ȡоƬ������ʼ��ַ */
unsigned long program_application_strtup_address(void)
{
	return(APPLICATION_ADDRESS);
}

/* ����洢���� */
static unsigned char program_memory_type;	

/* ������ݷ�Χ�Ƿ��ڿ�ʹ�÷�Χ�� */
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

/* ��ȡоƬ�ͳ��̵���Ϣ�ص����� */
static unsigned long read_infomation_callback(progam_flash_info_t infomaion)
{
	unsigned long result = 0;
	
	memcpy(infomaion, &progam_flash_infomation, sizeof(struct progam_flash_info));
	
	return result;
}

/* �ڴ�����ص����� */
static unsigned long erase_callback(memory_range_t range)
{
	if(memory_available(range))
	{
		memory_erase(range->address, range->length);
		return PROGRAM_EOK;
	}
	
	return PROGRAM_ERROR;
}

/* ��̻ص����� */
static unsigned long program_callback(memory_range_t range, unsigned char *buffer)
{
	if(memory_available(range))
	{
		memory_write(range->address, buffer, range->length);
		return PROGRAM_EOK;
	}
	
	
	return PROGRAM_ERROR;
}

/* ��ȡFlash���ݻص� */
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

/* ��ʵMemory���� */
static unsigned long verif_callback(memory_range_t range)
{
	if(memory_available(range))
	{
		return memory_crc32(range->address, range->length);
	}
	
	return 0;
}

/* ����Ӧ�� */
static unsigned long startup_application_callback(void)
{
	unsigned long result = 0;
	struct application_info info;

	if(program_memory_type != PROGRAM_MEMORY_TYPE_NOR)
	{
		/* ������������ */
		
		
		/* ���пռ䲻������ISP�����н��� */
		if((info.start < isp_iap_memory_range.address) && (info.start >= (isp_iap_memory_range.address + isp_iap_memory_range.length)))
		{
			/* �����������пռ� */
			copy_to_runmem(info.start, info.length);
			
			/* ����Ӧ�ó��� */
			jmp_application(info.start);
		}
	}
	else
	{
		/* ֱ��д���ݵ����пռ���Ҫָ��������ַ����Ӧ�ó���(��ǰ�����ַ�ǹ̶���) */
		jmp_application(program_application_strtup_address());
	}
	
	return result;
}

///* �ʷ����� */
//unsigned char lexical_analysis(char **next, char *word, unsigned char length)
//{
//	unsigned char count;
//	
//	count = 0;
//	*word = '\0';
//	
//	/* �����ո���Ʊ�� */
//	while(((**next == ' ') || (**next == '\t')) && (**next != '\0')) (*next)++;
//	
//	/* ��ȡ���� */
//	while((**next != '\0') && (**next != ' ') && (length--))
//	{
//		*word++ = **next;
//		(*next)++;
//		count++;
//	}
//	
//	/* ���ܻ�����������, ����û�б���ȡ����, ���ʱ��Ҫ�����������ʵ�λ�� */
//	while((**next != ' ') || (**next != '\t') || (**next != '\0')) (*next)++;
//	*word = '\0';
//	
//	return count;
//}

///* �﷨��� */
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

///* ������������ */
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

/* ����������Ϣ */
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

/* ��ʼ����̷����Ͳ��� */
void program_init(void)
{
	/* ��ʼ���洢�� */
	hardware_init();
	
	/* ���ò������� */
	program_protocol_callback_config((program_protol_callback_t)&callback_operation);
	program_memory_type = PROGRAM_MEMORY_TYPE_NOR;
}

