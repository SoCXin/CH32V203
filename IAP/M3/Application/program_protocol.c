#include <device_usart.h>
#include <define.h>
#include <program_protocol.h>
#include <soft_timer.h>
#include <device.h>

/* ����Э�鴦������ */
static unsigned char protocal_recv_buffer[PROTOCOL_RECV_BUFF_SIZE];
static unsigned char protocal_send_buffer[PROTOCOL_SEND_BUFF_SIZE];

/* ����Э��������ݻص����� */
static struct program_protol_callback program_protol_callback;

/* ����Э�鵱ǰ����״̬ */
enum program_status program_current_status;
enum program_start_status program_start_status_current;

unsigned char checksum(unsigned char *buff, unsigned long length)
{
	unsigned char sum = 0;
	
	while(length--) sum += *buff++;
	
	return sum;
}

/* ISP��̳�ʱ������ */
static void program_timeout(void *parameter)
{
	if(program_protol_callback.startup_application)
	{
		program_protol_callback.startup_application();
	}
}

/* ��ʼ��Э�� */
void program_protocol_init(void)
{
	memset(&program_protol_callback, 0, sizeof(struct program_protol_callback));
	program_current_status = PROGRAM_STATUS_STOP;
	
	/* ������ʱ��ʱ�� */
	soft_timer_start(PROGRAM_TIMEROUT_TIMER_ID, program_timeout, NULL, 500);
}

/* ����Э��ص����� */
void program_protocol_callback_config(const program_protol_callback_t callback)
{
	memcpy(&program_protol_callback, callback, sizeof(struct program_protol_callback));
};

/* ��׼�������� */
static void program_std_return(unsigned char cmd)
{
	unsigned long len = 0;
	
	protocal_send_buffer[len++] = 0x2A;
	protocal_send_buffer[len++] = cmd;
	protocal_send_buffer[len++] = cmd;
	protocal_send_buffer[len++] = 0x0A;
	usart_send(protocal_send_buffer, len);
}

/*
 * ��ȡ�����Ϣ����Щ��Ϣ����
 * 1. ISP IAP ����汾��
 * 2. MCU�ĳ��̺��ͺ���Ϣ
 * 3. �ɱ��FLASH�ķ�����
 * 4. ���̷�������ʼ�ͽ�����ַ
 */
static void program_read_infomation(unsigned char *buffer, unsigned long length)
{
	struct progam_flash_info progam_flash_info;
	unsigned short data_len;
	unsigned long len = 0;
	unsigned char i;
	
	/* ������ݳ��� */
	if(length != 4) return;
	
	if(program_protol_callback.read_infomation)
	{
		memset(&progam_flash_info, 0, sizeof(struct progam_flash_info));
		program_protol_callback.read_infomation(&progam_flash_info);
		
		protocal_send_buffer[len++] = 0x2A;
		protocal_send_buffer[len++] = PROGRAM_PROTOCOL_CMD_READ_INFO;
		
		/* �������ݳ��� */
		data_len = progam_flash_info.area_num;
		data_len = (data_len * 8) + 8;
		protocal_send_buffer[len++] = GET_BYTE(data_len, 1);
		protocal_send_buffer[len++] = GET_BYTE(data_len, 0);
		
		/* ��ȡ����汾�� */
		protocal_send_buffer[len++] = GET_BYTE(ISAP_IAP_VIRSION, 3);
		protocal_send_buffer[len++] = GET_BYTE(ISAP_IAP_VIRSION, 2);
		protocal_send_buffer[len++] = GET_BYTE(ISAP_IAP_VIRSION, 1);
		protocal_send_buffer[len++] = GET_BYTE(ISAP_IAP_VIRSION, 0);
		
		/* ��ȡMCU��Ϣ */
		protocal_send_buffer[len++] = GET_BYTE(MCU_CLASS_INFOMATION, 3);
		protocal_send_buffer[len++] = GET_BYTE(MCU_CLASS_INFOMATION, 2);
		protocal_send_buffer[len++] = GET_BYTE(MCU_CLASS_INFOMATION, 1);
		protocal_send_buffer[len++] = GET_BYTE(MCU_CLASS_INFOMATION, 0);
		
		/* ��ȡ�ɱ�̷����� */
		protocal_send_buffer[len++] = progam_flash_info.area_num;
		
		/* ��ȡ������Ϣ(��ʼ��ַ-����) */
		for(i = 0; i < progam_flash_info.area_num; i++)
		{
			protocal_send_buffer[len++] = GET_BYTE(progam_flash_info.range_list[i].address, 3);
			protocal_send_buffer[len++] = GET_BYTE(progam_flash_info.range_list[i].address, 2);
			protocal_send_buffer[len++] = GET_BYTE(progam_flash_info.range_list[i].address, 1);
			protocal_send_buffer[len++] = GET_BYTE(progam_flash_info.range_list[i].address, 0);
			
			protocal_send_buffer[len++] = GET_BYTE(progam_flash_info.range_list[i].length, 3);
			protocal_send_buffer[len++] = GET_BYTE(progam_flash_info.range_list[i].length, 2);
			protocal_send_buffer[len++] = GET_BYTE(progam_flash_info.range_list[i].length, 1);
			protocal_send_buffer[len++] = GET_BYTE(progam_flash_info.range_list[i].length, 0);
		}
		
		protocal_send_buffer[len] = checksum(&protocal_recv_buffer[1], len - 1);
		len++;
		protocal_send_buffer[len++] = 0x0A;
		usart_send(protocal_send_buffer, len);
	}
}

/*
 * Э�鴦�����оƬ, ����оƬʱָ�������ڴ�ķ�Χ
 */
static void program_erase(unsigned char *buffer, unsigned long length)
{
	struct memory_range range;
	unsigned long len = 0;
	unsigned char errno = 0;
	
	/* ������ݳ��� */
	if(length != 14) return;
	
	/* ��ȡ������ʼ��ַ */
	range.address = (range.address << 8) | buffer[4];
	range.address = (range.address << 8) | buffer[5];
	range.address = (range.address << 8) | buffer[6];
	range.address = (range.address << 8) | buffer[7];
	
	/* ��ȡ�������� */
	range.length = (range.length << 8) | buffer[8];
	range.length = (range.length << 8) | buffer[9];
	range.length = (range.length << 8) | buffer[10];
	range.length = (range.length << 8) | buffer[11];
	
	/* ִ�в��� */
	if(program_protol_callback.erase)
	{
		errno = program_protol_callback.erase(&range);
	}
	
	/* �������ݰ� */
	protocal_send_buffer[len++] = 0x2A;
	protocal_send_buffer[len++] = PROGRAM_PROTOCOL_CMD_ERASE;
	protocal_send_buffer[len++] = 0x00;
	protocal_send_buffer[len++] = 0x01;
	protocal_send_buffer[len++] = errno;
	protocal_send_buffer[len] = checksum(&protocal_recv_buffer[1], len - 1);
	len++;
	protocal_send_buffer[len++] = 0x0A;
	usart_send(protocal_send_buffer, len);
}

/* 
 * Э��ִ�б��, ����PC�˷��͵ı����Ϣִ�б��
 */
static void program_program(unsigned char *buffer, unsigned long length)
{
	struct memory_range range;
	unsigned long len = 0;
	unsigned char errno = 0;
	unsigned short data_length;
	
	/* ������ݳ��� */
	if(length < 6) return;
	data_length = buffer[2];
	data_length = (data_length << 8) | buffer[3];
	if((data_length + 6) != length) return;
	
	/* ��ȡ�����ʼ��ַ */
	range.address = (range.address << 8) | buffer[4];
	range.address = (range.address << 8) | buffer[5];
	range.address = (range.address << 8) | buffer[6];
	range.address = (range.address << 8) | buffer[7];
	
	/* ��ȡ������ݳ��� */
	range.length = 0;
	range.length = (range.length << 8) | buffer[8];
	range.length = (range.length << 8) | buffer[9];
	
	/* ִ�б�̲��� */
	if(program_protol_callback.program)
	{
		errno = program_protol_callback.program(&range, &buffer[10]);
	}
	
	/* �������ݰ���PC */
	protocal_send_buffer[len++] = 0x2A;
	protocal_send_buffer[len++] = PROGRAM_PROTOCOL_CMD_PROGRAM;
	protocal_send_buffer[len++] = 0x00;
	protocal_send_buffer[len++] = 0x01;
	protocal_send_buffer[len++] = errno;
	protocal_send_buffer[len] = checksum(&protocal_recv_buffer[1], len - 1);
	len++;
	protocal_send_buffer[len++] = 0x0A;
	usart_send(protocal_send_buffer, len);
}

/* ����PC�·���Ϣ��ȡMemory�е����� */
static void program_read(unsigned char *buffer, unsigned long length)
{
	struct memory_range range;
	unsigned long len = 0;
	unsigned short read_len = 0;
	unsigned short data_length;
	
	/* ������ݳ��� */
	if(length < 6) return;
	data_length = buffer[2];
	data_length = (data_length << 8) | buffer[3];
	if((data_length + 6) != length) return;
	
	/* ��ȡ��ȡ��ʼ��ַ */
	range.address = (range.address << 8) | buffer[4];
	range.address = (range.address << 8) | buffer[5];
	range.address = (range.address << 8) | buffer[6];
	range.address = (range.address << 8) | buffer[7];
	
	/* ��ȡ���ȡ���ݳ��� */
	range.length = 0;
	range.length = (range.length << 8) | buffer[8];
	range.length = (range.length << 8) | buffer[9];
	
	/* ִ�ж�ȡ���� */
	if(program_protol_callback.read)
	{
		read_len = program_protol_callback.read(&protocal_send_buffer[4], 1024, &range);
	}
	
	/* �������ݰ���PC */
	protocal_send_buffer[len++] = 0x2A;
	protocal_send_buffer[len++] = PROGRAM_PROTOCOL_CMD_READ;
	protocal_send_buffer[len++] = GET_BYTE(read_len, 1);
	protocal_send_buffer[len++] = GET_BYTE(read_len, 0);
	len += read_len;
	protocal_send_buffer[len] = checksum(&protocal_recv_buffer[1], len - 1);
	len++;
	protocal_send_buffer[len++] = 0x0A;
	usart_send(protocal_send_buffer, len);
}

/* ����PCָ������Ϣ����Memory�����ݵ�CRCУ�鲢���ظ�PC */
static void program_verif(unsigned char *buffer, unsigned long length)
{
	struct memory_range range;
	unsigned long len = 0;
	unsigned long check_crc = 0;
	unsigned short data_length;
	
	/* ������ݳ��� */
	if(length < 6) return;
	data_length = buffer[2];
	data_length = (data_length << 8) | buffer[3];
	if((data_length + 6) != length) return;
	
	/* ��ȡ��ȡ��ʼ��ַ */
	range.address = (range.address << 8) | buffer[4];
	range.address = (range.address << 8) | buffer[5];
	range.address = (range.address << 8) | buffer[6];
	range.address = (range.address << 8) | buffer[7];
	
	/* ��ȡ���ȡ���ݳ��� */
	range.length = 0;
	range.length = (range.length << 8) | buffer[8];
	range.length = (range.length << 8) | buffer[9];
	
	/* ִ�б�̲��� */
	if(program_protol_callback.verif)
	{
		check_crc = program_protol_callback.verif(&range);
	}
	
	/* �������ݰ���PC */
	protocal_send_buffer[len++] = 0x2A;
	protocal_send_buffer[len++] = PROGRAM_PROTOCOL_CMD_VERIF;
	protocal_send_buffer[len++] = 0x00;
	protocal_send_buffer[len++] = 0x04;
	protocal_send_buffer[len++] = GET_BYTE(check_crc, 3);
	protocal_send_buffer[len++] = GET_BYTE(check_crc, 2);
	protocal_send_buffer[len++] = GET_BYTE(check_crc, 1);
	protocal_send_buffer[len++] = GET_BYTE(check_crc, 0);
	protocal_send_buffer[len] = checksum(&protocal_recv_buffer[1], len - 1);
	len++;
	protocal_send_buffer[len++] = 0x0A;
	usart_send(protocal_send_buffer, len);
}

/* Э������APP���� */
static void program_startup(unsigned char *buffer, unsigned long length)
{
	if(length != 4) return;
	
	/* ִ�б�̲��� */
	if(program_protol_callback.startup_application)
	{
		// ����Ӧ��ǰ�رն�ʱ��
		soft_timer_stop(PROGRAM_TIMEROUT_TIMER_ID);
		program_protol_callback.startup_application();
	}
	
	/* �������ݰ���PC */
	program_std_return(PROGRAM_PROTOCOL_CMD_STARTUP);
}

/* ������������ */
static void program_bootarg(unsigned char *buffer, unsigned long length)
{
	unsigned short data_length;
	
	/* ������ݳ��� */
	if(length < 6) return;
	data_length = buffer[2];
	data_length = (data_length << 8) | buffer[3];
	if((data_length + 6) != length) return;
	
	if(program_protol_callback.bootarg_set)
	{
		program_protol_callback.bootarg_set((const char *)&buffer[4]);
	}
	
	/* �������ݰ���PC */
	program_std_return(PROGRAM_PROTOCOL_CMD_BOOTR);
}

/* �����ڱ��̽���ź� */
static unsigned char program_probe(unsigned char chr)
{
	if(chr == 0xA5)
	{
		protocal_send_buffer[0] = 0x5A;
		usart_send(protocal_send_buffer, 1);
		return True;
	}
	
	return False;
}

/* Э�鴦����ִ���� */
static unsigned char program_probe_stream(unsigned char *buffer, unsigned long length)
{
	unsigned char result;
	
	if(program_start_status_current == PROGRAM_START_SREAM0)
	{
		result = PROGRAM_START_SREAM0;
		
		if(length != strlen(PROGRAM_SATRT_DATA_STREAM0)) 
		{
			program_start_status_current = PROGRAM_START_SREAM0;
		}
		else if(memcmp(buffer, PROGRAM_SATRT_DATA_STREAM0, strlen(PROGRAM_SATRT_DATA_STREAM0)))
		{
			program_start_status_current = PROGRAM_START_SREAM0;
		}
		else
		{
			soft_timer_start(PROGRAM_TIMEROUT_TIMER_ID, program_timeout, NULL, 500);
			memcpy(protocal_send_buffer, PROGRAM_SATRT_DATA_STREAM1, strlen(PROGRAM_SATRT_DATA_STREAM1));
			usart_send(protocal_send_buffer, strlen(PROGRAM_SATRT_DATA_STREAM1));
			program_start_status_current = PROGRAM_START_SREAM1;
		}
	}
	else if(program_start_status_current == PROGRAM_START_SREAM1)
	{
		result = PROGRAM_START_SREAM1;
		
		if(length != (strlen(PROGRAM_SATRT_DATA_STREAM1))) 
		{
			program_start_status_current = PROGRAM_START_SREAM0;
		}
		else if(memcmp(buffer, PROGRAM_SATRT_DATA_STREAM1, strlen(PROGRAM_SATRT_DATA_STREAM1))) 
		{
			program_start_status_current = PROGRAM_START_SREAM0;
		}
		else
		{
			soft_timer_start(PROGRAM_TIMEROUT_TIMER_ID, program_timeout, NULL, 500);
			memcpy(protocal_send_buffer, PROGRAM_SATRT_DATA_STREAM0, strlen(PROGRAM_SATRT_DATA_STREAM0));
			usart_send(protocal_send_buffer, strlen(PROGRAM_SATRT_DATA_STREAM0));
			program_start_status_current = PROGRAM_START_OK;
		}
	}
	else
	{
		result = PROGRAM_START_OK;
	}
	
	return result;
}

/* ������Э�� */
static void program_protocol(unsigned char *buffer, unsigned long length)
{
	/* У�����ָ���, �������������ֱ�Ӷ��ص�������� */
	if(length < 4) return;
	
	/* У���ͷ�Ͱ�β, ��ͷ��β��������ֱ�Ӷ�����һ������ */
	if(protocal_recv_buffer[0] != 0x3A || protocal_recv_buffer[length - 1] != 0x0A)
	{
		return;
	}
	
	/* У��ͼ��, У��ʹ���ֱ�Ӷ������� */
	if(protocal_recv_buffer[length - 2] != checksum(&protocal_recv_buffer[1], length - 3))
	{
		return;
	}
	
	/* ����ǲ��ɲ����ľ�ֻ����ǲ��������ñ��״̬�����ݰ�, 
	 * ���յ��趨״̬�����ݰ���������Ӧ�Ĵ���״̬, �������
	 * �ɱ�̵ģ��Ϳ��Դ������е�������Ϣ��
	 */
	if(program_current_status == PROGRAM_STATUS_STOP)
	{
		/* ������ݳ��� */
		if((length != 7) && (protocal_recv_buffer[1] != PROGRAM_PROTOCOL_CMD_STATUS)) return;
		
		if(protocal_recv_buffer[4] == PROGRAM_PROTOCOL_CMD_STATUS)
		{
			program_current_status = PROGRAM_STATUS_SATRT;
			
			// ������ģʽ����������ʱ��ʱ��, ��ʱʱ������Ϊ3000ms
			soft_timer_start(PROGRAM_TIMEROUT_TIMER_ID, program_timeout, NULL, 5000);
		}
		else
		{
			program_current_status = PROGRAM_STATUS_STOP;
			
			// �˳����ģʽ����������ʱ��ʱ��, ��ʱʱ������Ϊ500ms
			soft_timer_start(PROGRAM_TIMEROUT_TIMER_ID, program_timeout, NULL, 500);
		}
		
		/* �������ݰ���PC */
		program_std_return(PROGRAM_PROTOCOL_CMD_STATUS);
	}
	else if(program_current_status == PROGRAM_STATUS_SATRT)
	{
		// һֱ�����ݽ���������������ʱ��
		soft_timer_start(PROGRAM_TIMEROUT_TIMER_ID, program_timeout, NULL, 5000);
			
		switch(protocal_recv_buffer[1])
		{
		case PROGRAM_PROTOCOL_CMD_READ_INFO:
			program_read_infomation(protocal_recv_buffer, length);
			break;
			
		case PROGRAM_PROTOCOL_CMD_ERASE:
			program_erase(protocal_recv_buffer, length);
			break;
			
		case PROGRAM_PROTOCOL_CMD_PROGRAM:
			program_program(protocal_recv_buffer, length);
			break;
			
		case PROGRAM_PROTOCOL_CMD_READ:
			program_read(protocal_recv_buffer, length);
			break;
			
		case PROGRAM_PROTOCOL_CMD_VERIF:
			program_verif(protocal_recv_buffer, length);
			break;
			
		case PROGRAM_PROTOCOL_CMD_STARTUP:
			program_startup(protocal_recv_buffer, length);
			break;
		case PROGRAM_PROTOCOL_CMD_BOOTR:
			program_bootarg(protocal_recv_buffer, length);
			break;
			
		default:
			break;
		}
	}
}

/* Э�鴦�� */
void program_protocol_handler(void)
{
	unsigned long length;
	
	if(usart_status_get(USART_STATUS_RECV_FINISH))
	{
		led_control(LED_ON);
	
		do
		{
			/* ��ȡ�������� */
			length = usart_read(protocal_recv_buffer, PROTOCOL_RECV_BUFF_SIZE);
			
			/* ����̽���ź� */
			if(program_probe(protocal_recv_buffer[0]) == True) break;
		
			/* �������������ź� */
			if(program_probe_stream(protocal_recv_buffer, length) != PROGRAM_START_OK) break;
			
			/* ����Э�� */
			program_protocol(protocal_recv_buffer, length);
		}while(0);
		
		usart_status_clear(USART_STATUS_RECV_FINISH);
		led_control(LED_OFF);
	}
}

