#include <device_usart.h>
#include <define.h>
#include <program_protocol.h>
#include <soft_timer.h>
#include <device.h>

/* 定义协议处理缓冲区 */
static unsigned char protocal_recv_buffer[PROTOCOL_RECV_BUFF_SIZE];
static unsigned char protocal_send_buffer[PROTOCOL_SEND_BUFF_SIZE];

/* 定义协议具体内容回调函数 */
static struct program_protol_callback program_protol_callback;

/* 定义协议当前处理状态 */
enum program_status program_current_status;
enum program_start_status program_start_status_current;

unsigned char checksum(unsigned char *buff, unsigned long length)
{
	unsigned char sum = 0;
	
	while(length--) sum += *buff++;
	
	return sum;
}

/* ISP编程超时处理函数 */
static void program_timeout(void *parameter)
{
	if(program_protol_callback.startup_application)
	{
		program_protol_callback.startup_application();
	}
}

/* 初始化协议 */
void program_protocol_init(void)
{
	memset(&program_protol_callback, 0, sizeof(struct program_protol_callback));
	program_current_status = PROGRAM_STATUS_STOP;
	
	/* 启动超时定时器 */
	soft_timer_start(PROGRAM_TIMEROUT_TIMER_ID, program_timeout, NULL, 500);
}

/* 设置协议回调函数 */
void program_protocol_callback_config(const program_protol_callback_t callback)
{
	memcpy(&program_protol_callback, callback, sizeof(struct program_protol_callback));
};

/* 标准返回数据 */
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
 * 读取编程信息，这些信息包括
 * 1. ISP IAP 软件版本号
 * 2. MCU的厂商和型号信息
 * 3. 可编程FLASH的分区数
 * 4. 编编程分区的起始和结束地址
 */
static void program_read_infomation(unsigned char *buffer, unsigned long length)
{
	struct progam_flash_info progam_flash_info;
	unsigned short data_len;
	unsigned long len = 0;
	unsigned char i;
	
	/* 检测数据长度 */
	if(length != 4) return;
	
	if(program_protol_callback.read_infomation)
	{
		memset(&progam_flash_info, 0, sizeof(struct progam_flash_info));
		program_protol_callback.read_infomation(&progam_flash_info);
		
		protocal_send_buffer[len++] = 0x2A;
		protocal_send_buffer[len++] = PROGRAM_PROTOCOL_CMD_READ_INFO;
		
		/* 计算数据长度 */
		data_len = progam_flash_info.area_num;
		data_len = (data_len * 8) + 8;
		protocal_send_buffer[len++] = GET_BYTE(data_len, 1);
		protocal_send_buffer[len++] = GET_BYTE(data_len, 0);
		
		/* 读取软件版本号 */
		protocal_send_buffer[len++] = GET_BYTE(ISAP_IAP_VIRSION, 3);
		protocal_send_buffer[len++] = GET_BYTE(ISAP_IAP_VIRSION, 2);
		protocal_send_buffer[len++] = GET_BYTE(ISAP_IAP_VIRSION, 1);
		protocal_send_buffer[len++] = GET_BYTE(ISAP_IAP_VIRSION, 0);
		
		/* 读取MCU信息 */
		protocal_send_buffer[len++] = GET_BYTE(MCU_CLASS_INFOMATION, 3);
		protocal_send_buffer[len++] = GET_BYTE(MCU_CLASS_INFOMATION, 2);
		protocal_send_buffer[len++] = GET_BYTE(MCU_CLASS_INFOMATION, 1);
		protocal_send_buffer[len++] = GET_BYTE(MCU_CLASS_INFOMATION, 0);
		
		/* 读取可编程分区数 */
		protocal_send_buffer[len++] = progam_flash_info.area_num;
		
		/* 读取分区信息(起始地址-长度) */
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
 * 协议处理擦除芯片, 擦除芯片时指定擦除内存的范围
 */
static void program_erase(unsigned char *buffer, unsigned long length)
{
	struct memory_range range;
	unsigned long len = 0;
	unsigned char errno = 0;
	
	/* 检测数据长度 */
	if(length != 14) return;
	
	/* 获取擦除起始地址 */
	range.address = (range.address << 8) | buffer[4];
	range.address = (range.address << 8) | buffer[5];
	range.address = (range.address << 8) | buffer[6];
	range.address = (range.address << 8) | buffer[7];
	
	/* 获取擦除长度 */
	range.length = (range.length << 8) | buffer[8];
	range.length = (range.length << 8) | buffer[9];
	range.length = (range.length << 8) | buffer[10];
	range.length = (range.length << 8) | buffer[11];
	
	/* 执行擦除 */
	if(program_protol_callback.erase)
	{
		errno = program_protol_callback.erase(&range);
	}
	
	/* 返回数据包 */
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
 * 协议执行编程, 根据PC端发送的编程信息执行编程
 */
static void program_program(unsigned char *buffer, unsigned long length)
{
	struct memory_range range;
	unsigned long len = 0;
	unsigned char errno = 0;
	unsigned short data_length;
	
	/* 检测数据长度 */
	if(length < 6) return;
	data_length = buffer[2];
	data_length = (data_length << 8) | buffer[3];
	if((data_length + 6) != length) return;
	
	/* 获取编程起始地址 */
	range.address = (range.address << 8) | buffer[4];
	range.address = (range.address << 8) | buffer[5];
	range.address = (range.address << 8) | buffer[6];
	range.address = (range.address << 8) | buffer[7];
	
	/* 获取编程数据长度 */
	range.length = 0;
	range.length = (range.length << 8) | buffer[8];
	range.length = (range.length << 8) | buffer[9];
	
	/* 执行编程操作 */
	if(program_protol_callback.program)
	{
		errno = program_protol_callback.program(&range, &buffer[10]);
	}
	
	/* 返回数据包到PC */
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

/* 根据PC下发信息读取Memory中的数据 */
static void program_read(unsigned char *buffer, unsigned long length)
{
	struct memory_range range;
	unsigned long len = 0;
	unsigned short read_len = 0;
	unsigned short data_length;
	
	/* 检测数据长度 */
	if(length < 6) return;
	data_length = buffer[2];
	data_length = (data_length << 8) | buffer[3];
	if((data_length + 6) != length) return;
	
	/* 获取读取起始地址 */
	range.address = (range.address << 8) | buffer[4];
	range.address = (range.address << 8) | buffer[5];
	range.address = (range.address << 8) | buffer[6];
	range.address = (range.address << 8) | buffer[7];
	
	/* 获取编读取数据长度 */
	range.length = 0;
	range.length = (range.length << 8) | buffer[8];
	range.length = (range.length << 8) | buffer[9];
	
	/* 执行读取操作 */
	if(program_protol_callback.read)
	{
		read_len = program_protol_callback.read(&protocal_send_buffer[4], 1024, &range);
	}
	
	/* 返回数据包到PC */
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

/* 根据PC指定的信息计算Memory中数据的CRC校验并返回给PC */
static void program_verif(unsigned char *buffer, unsigned long length)
{
	struct memory_range range;
	unsigned long len = 0;
	unsigned long check_crc = 0;
	unsigned short data_length;
	
	/* 检测数据长度 */
	if(length < 6) return;
	data_length = buffer[2];
	data_length = (data_length << 8) | buffer[3];
	if((data_length + 6) != length) return;
	
	/* 获取读取起始地址 */
	range.address = (range.address << 8) | buffer[4];
	range.address = (range.address << 8) | buffer[5];
	range.address = (range.address << 8) | buffer[6];
	range.address = (range.address << 8) | buffer[7];
	
	/* 获取编读取数据长度 */
	range.length = 0;
	range.length = (range.length << 8) | buffer[8];
	range.length = (range.length << 8) | buffer[9];
	
	/* 执行编程操作 */
	if(program_protol_callback.verif)
	{
		check_crc = program_protol_callback.verif(&range);
	}
	
	/* 返回数据包到PC */
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

/* 协议启动APP运行 */
static void program_startup(unsigned char *buffer, unsigned long length)
{
	if(length != 4) return;
	
	/* 执行编程操作 */
	if(program_protol_callback.startup_application)
	{
		// 启动应用前关闭定时器
		soft_timer_stop(PROGRAM_TIMEROUT_TIMER_ID);
		program_protol_callback.startup_application();
	}
	
	/* 返回数据包到PC */
	program_std_return(PROGRAM_PROTOCOL_CMD_STARTUP);
}

/* 设置启动参数 */
static void program_bootarg(unsigned char *buffer, unsigned long length)
{
	unsigned short data_length;
	
	/* 检测数据长度 */
	if(length < 6) return;
	data_length = buffer[2];
	data_length = (data_length << 8) | buffer[3];
	if((data_length + 6) != length) return;
	
	if(program_protol_callback.bootarg_set)
	{
		program_protol_callback.bootarg_set((const char *)&buffer[4]);
	}
	
	/* 返回数据包到PC */
	program_std_return(PROGRAM_PROTOCOL_CMD_BOOTR);
}

/* 处理串口编程探测信号 */
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

/* 协议处理编程执行流 */
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

/* 处理编程协议 */
static void program_protocol(unsigned char *buffer, unsigned long length)
{
	/* 校验基本指令长度, 不满足基本长度直接丢地道这包数据 */
	if(length < 4) return;
	
	/* 校验包头和包尾, 包头包尾不满足则直接丢弃这一包数据 */
	if(protocal_recv_buffer[0] != 0x3A || protocal_recv_buffer[length - 1] != 0x0A)
	{
		return;
	}
	
	/* 校验和检查, 校验和错误直接丢掉数据 */
	if(protocal_recv_buffer[length - 2] != checksum(&protocal_recv_buffer[1], length - 3))
	{
		return;
	}
	
	/* 编程是不可操作的就只检测是不是有设置编程状态的数据包, 
	 * 接收到设定状态的数据包则设置相应的处理状态, 否则就是
	 * 可编程的，就可以处理所有的命令消息了
	 */
	if(program_current_status == PROGRAM_STATUS_STOP)
	{
		/* 检测数据长度 */
		if((length != 7) && (protocal_recv_buffer[1] != PROGRAM_PROTOCOL_CMD_STATUS)) return;
		
		if(protocal_recv_buffer[4] == PROGRAM_PROTOCOL_CMD_STATUS)
		{
			program_current_status = PROGRAM_STATUS_SATRT;
			
			// 进入编程模式重新启动超时定时器, 定时时间设置为3000ms
			soft_timer_start(PROGRAM_TIMEROUT_TIMER_ID, program_timeout, NULL, 5000);
		}
		else
		{
			program_current_status = PROGRAM_STATUS_STOP;
			
			// 退出编程模式重新启动超时定时器, 定时时间设置为500ms
			soft_timer_start(PROGRAM_TIMEROUT_TIMER_ID, program_timeout, NULL, 500);
		}
		
		/* 返回数据包到PC */
		program_std_return(PROGRAM_PROTOCOL_CMD_STATUS);
	}
	else if(program_current_status == PROGRAM_STATUS_SATRT)
	{
		// 一直有数据交换就重新启动定时器
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

/* 协议处理 */
void program_protocol_handler(void)
{
	unsigned long length;
	
	if(usart_status_get(USART_STATUS_RECV_FINISH))
	{
		led_control(LED_ON);
	
		do
		{
			/* 获取串口数据 */
			length = usart_read(protocal_recv_buffer, PROTOCOL_RECV_BUFF_SIZE);
			
			/* 处理探测信号 */
			if(program_probe(protocal_recv_buffer[0]) == True) break;
		
			/* 处理变成启动流信号 */
			if(program_probe_stream(protocal_recv_buffer, length) != PROGRAM_START_OK) break;
			
			/* 处理协议 */
			program_protocol(protocal_recv_buffer, length);
		}while(0);
		
		usart_status_clear(USART_STATUS_RECV_FINISH);
		led_control(LED_OFF);
	}
}

