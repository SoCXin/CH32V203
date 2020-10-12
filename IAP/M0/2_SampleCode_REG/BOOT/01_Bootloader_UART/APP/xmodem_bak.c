//*================================================================================================
//* 文件名               : xmodem.c
//* 功能描述            : Xmodem协议处理相关函数
//* 作者              : MM32 MCU
//* 版本              : 0.01
//* 建立日期、时间       : 2020/05/14 13:30
//* 最近修改日期、时间 :
//* 修改原因            :
//*================================================================================================
/*------------------------------------------ 头文件 ---------------------------------------------*/
#include    "xmodem.h"
#include    "flash.h"
#include    "tim3.h"
#include    "uart.h"
#include    "string.h"
#include    <stdio.h>
/*-----------------------------------------------------------------------------------------------*/

/*----------------------------------------- 全局变量 --------------------------------------------*/
#define     PROM_ADDR   0x8001400

FLASH_Status FLASHStatus = FLASH_COMPLETE	;
#define PageSize   (1024)          //字节
uint8_t EraseCounter = 0 ;

uint32_t g_RequestTimes = 0; //发送文件请求次数
uint8_t g_CheckType = 'C';  //接收文件的校验方式
uint8_t g_HexDataTemp[153];//接收HEX文件缓冲区	  1530

/* CRC16 implementation acording to CCITT standards */
static const unsigned short crc16tab[256] =
{
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
    0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
    0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
    0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
    0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
    0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
    0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
    0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
    0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
    0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
    0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
    0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
    0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
    0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
    0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
    0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
    0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
    0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
    0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
    0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
    0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
    0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
    0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
    0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
    0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
    0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
    0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
    0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
    0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
    0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
    0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
    0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
};
unsigned short crc16_ccitt(const unsigned char *buf, int len)
{
    register int counter;
    register unsigned short crc = 0;
    for (counter = 0; counter < len; counter++)
        crc = (crc << 8) ^ crc16tab[((crc >> 8) ^ * (char *)buf++) & 0x00FF];
    return crc;
}



/**************************************************************************************************
** 函数名称 : check
** 功能描述 : 校验，CRC为真则为CRC校验，否则为校验和
** 入口参数 : <crc>[in] 选择是CRC校验还是SUM校验
**            <buf>[in] 校验的原始数据
**            <sz>[in]  校验的数据长度
** 出口参数 : 无
** 返 回 值 : 校验无误返回TURE,反之返回FALSE
** 其他说明 : 无
***************************************************************************************************/
int check(int crc, const unsigned char *buf, int sz)
{
    if (crc)
    {
        unsigned short crc = crc16_ccitt(buf, sz);
        unsigned short tcrc = (buf[sz] << 8) + buf[sz + 1];
        if (crc == tcrc)
            return TRUE;
    }
    else
    {
        int i;
        unsigned char cks = 0;
        for (i = sz; i != 0; i--)
        {
            cks += *(buf++);
        }
        if (cks == *buf)
            return TRUE;
    }
    return FALSE;
}



uint8_t uc1A_Count = 0 ;

/**************************************************************************************************
** 函数名称 : xmodemReceive
** 功能描述 : xmodem协议接收接收文件
** 入口参数 : <checkType>[in] 接收文件的校验方式，'C':crc校验，NAK:累加和校验
** 出口参数 : 无
** 返 回 值 : 接收文件操作时的相关错误代码
** 其他说明 : 无
***************************************************************************************************/
uint8_t xmodemReceive(uint8_t checkType)
{
    uint8_t xbuff[133]; /* 128 for XModem  + 3 head chars + 2 crc + nul */
    uint8_t crc = 1;//启用CRC校验
    uint8_t packetno = 1,pageoffsetno = 0 ;
    uint8_t bufsz;
    uint32_t i = 0;
    uint32_t j = 0;
//	uint8_t ErrorCode = 0;
    uint8_t *p_LastData;
//    uint8_t *p_DataType;
    uint8_t chTemp = 0; //接收字符缓存
//  volatile uint32_t   RowNum2=0;
//    uint8_t RowNum = 0; //接收到的行数
	uint8_t uc_isLastPac = 0 ;
//	int32_t packet_length;
//	uint32_t RamSource;
	
    p_LastData = g_HexDataTemp;
//    p_DataType = g_HexDataTemp;
	
    if (checkType == CHECK_CRC) //CRC校验
    {
        crc = 1;
        g_CheckType = 'C';
    }
    else if (checkType == CHECK_SUM)
    {
        crc = 0;
        g_CheckType = NAK;
    }
    while (1)
    {
        bufsz = 128;
        SerialPutString(IAP_UART,"\n\r->");
        //向上位机请求发送文件
        for (i = 5000; i != 0; i--) //5000
        {
            chTemp = UARTx_ReadByte(IAP_UART,500*13); //根据实际情况调整超时大小 500ms = 48*1000*500
            if (chTemp > 0)
            {
                TIM3->CR1 &= ((uint16_t)0xFFFE);  /* Disable the TIM Counter */
				TIM3->CNT = 0;
                break;
            }
            else
            {
                UARTx_WriteByte(IAP_UART,g_CheckType);
            }
        }
        //文件传输超时
        if ((chTemp == 0) && (i == 0))
        {
            SerialPutString(IAP_UART,"\n\r->Transmit Time Out!!");
            SerialPutString(IAP_UART,"\n\r->");
            /*此处可以返回错误信息*/
            return TIME_OUT;
        }
        //用户取消文件传输
        else if ((chTemp == 'B') || (chTemp == 'b'))
        {
//            SerialPutString("\n\r->%c", chTemp);
            SerialPutString(IAP_UART,"\n\r->User Canceled!!");
            SerialPutString(IAP_UART,"\n\r->");
            /*此处可以返回错误信息*/
            return USER_CANCELED;
        }
        else  //开始传输文件
        {
            while (chTemp == SOH) //接收到有效数据帧头
            {
                xbuff[0] = chTemp;
                for (i = 0; i < sizeof(xbuff); i++) //接收一帧数据
                {
                    xbuff[i + 1] = UARTx_ReadByte(IAP_UART,1*13); //根据实际情况调整超时大小 1ms = 48*1000*1
                }

                if ((xbuff[1] == (uint8_t)~xbuff[2]) && (packetno == xbuff[1]) //包序号无误
                        && (check(crc, &xbuff[3], bufsz))) //CRC校验无误
                {
					//擦除Flash
					if(FLASHStatus == FLASH_COMPLETE)
					{
						if((packetno-1)%8 == 0)
						{
							FLASH_Unlock();
//							FLASH->KEYR = (u32)0x45670123;
//							FLASH->KEYR = (u32)0xCDEF89AB;
							FLASHStatus = FLASH_ErasePage(APPLICATION_ADDRESS+ (PageSize *EraseCounter));
							FLASH_Lock();
//							FLASH->CR |= 1 << 7 ;
							EraseCounter++;
						}
					}
                    packetno++;
                    for (j = 0; (j < 128) ;j++)
                    {
//                        if (xbuff[3 + j] == ':')
//                        {
//                            p_DataType = p_LastData;
//                        }
//                        else if (xbuff[3 + j] == 0x0A) //接收到换行符
//                        {
//                            if (*(p_DataType + 8) == '0')
//                            {
//                                RowNum++; //(((8+k)>=128)?((k+8)-128):(8+k))
//                            }
//                        }
                        *(p_LastData++) = xbuff[3 + j];
//                        j++;						
						if( (CTRLZ == xbuff[5 + j]) && (xbuff[4 + j] == xbuff[5 + j]) && (xbuff[3 + j] == xbuff[4 + j]) )		
						{	
							uc1A_Count= (3+j) ;
							uc_isLastPac = 1 ;
							break;
						}
                    }				
					if(uc_isLastPac)
					{
						Iap_WriteAppBin( (APPLICATION_ADDRESS + pageoffsetno * 128),  g_HexDataTemp,  uc1A_Count-2) ; //uc1A_Count-2
//                        p_LastData = g_HexDataTemp;
					}
					else{
						Iap_WriteAppBin( (APPLICATION_ADDRESS + pageoffsetno * 128),  g_HexDataTemp,  128) ;
                        p_LastData = g_HexDataTemp;
						pageoffsetno ++ ;
					}
                    UARTx_WriteByte(IAP_UART,ACK);
                }
                else //要求重发
                {
                    UARTx_WriteByte(IAP_UART,NAK);
                }
                do
                {
                    chTemp = UARTx_ReadByte(IAP_UART,10*13);//读取下一帧数据的帧头,//根据实际情况调整超时大小 10ms = 48*1000*10
                }
                while (chTemp == 0);
            }
            UARTx_WriteByte(IAP_UART,chTemp);
            if (chTemp == EOT) //文件发送结束标志
            {
                UARTx_WriteByte(IAP_UART,ACK);
//                /*烧写最后一帧数据*/
//                if (RowNum < 16)
//                {
//                    *p_LastData = 0x0A;
//                    ProgramHexToFlash(g_HexDataTemp, USE_DFT_ADDR, APPLICATION_ADDRESS);
//                }
				
                SerialPutString(IAP_UART,"\n\r->Load Hex File To Flash Of CRC OK ^_^");
                SerialPutString(IAP_UART,"\n\r->");
            }

            chTemp = UARTx_ReadByte(IAP_UART,50*13); //根据实际情况调整超时大小 50ms = 48*1000*50
            if (chTemp == EOT) //文件发送结束标志
            {
                UARTx_WriteByte(IAP_UART,ACK);
                SerialPutString(IAP_UART,"\n\r->Load Hex File To Flash Of CRC OK ^_^");
                SerialPutString(IAP_UART,"\n\r->");
            }
            /*此处可以返回错误信息*/
            return SUCCESSFULL;
        }
    }
}

