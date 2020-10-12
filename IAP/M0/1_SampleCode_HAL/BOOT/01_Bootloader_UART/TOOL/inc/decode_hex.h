//*================================================================================================
//* �ļ���              : decodeHex.h
//* ��������            : ����Hex�ļ�����غ������������ݶ���
//* ����                : ��ΰ��
//* �汾                : 0.01
//* �������ڡ�ʱ��      : 2011/04/13 21:27
//* ����޸����ڡ�ʱ��  :
//* �޸�ԭ��            :
//*================================================================================================
#ifndef __DECODEHEX_H
#define __DECODEHEX_H
#include "HAL_device.h"
#ifndef NULL
    #define NULL    ((void *)0)
#endif
#ifndef FALSE
    #define FALSE   (0)
#endif
#ifndef TRUE
    #define TRUE    (1)
#endif
#ifndef ERROR
    #define ERROR    (255)
#endif
#ifndef RIGHT
    #define RIGHT    (0)
#endif

typedef struct
{
    uint32_t    DataLength;                 /*��������¼�ĳ���������ʾ����(dd)���ֽ���Ŀ          */
    uint32_t    AddrBase;                   /*����ַ��Ϣ                                            */
    uint32_t    OffsetAddr;                 /*ƫ�Ƶ�ַ��Ϣ                                          */
    uint32_t    Rectype;                    /*������ʾ����HEX��¼������                           */
    uint32_t    SumCheckOk;                 /*���ݺ�У����ȷ                                        */
    uint8_t *p_Data;                    /*������ָ��                                            */
    //uint8_t   *p_NextLineAddr;            /*��һ��hex����ָ��                                     */
} t_HexLineInfo;

/*Rectype��������ȡֵ*/
#define     RCD_DATA                '0' /*������¼���ݣ�HEX�ļ��Ĵ󲿷ּ�¼�������ݼ�¼         */
#define     RCD_END_FILE            '1' /*������ʶ�ļ������������ļ�����󣬱�ʶHEX�ļ��Ľ�β   */
#define     RCD_EXT_SEGMENT_ADDR    '2' /*������ʶ��չ�ε�ַ�ļ�¼                              */
#define     RCD_START_SEGMENT_ADDR  '3' /*������ʶ��ʼ�ε�ַ�ļ�¼                              */
#define     RCD_EXT_LINEAR_ADDR     '4' /*������ʶ��չ���Ե�ַ�ļ�¼                            */
#define     RCD_START_LINEAR_ADDR   '5' /*������ʶ��ʼ���Ե�ַ�ļ�¼                            */
#define     RCD_ERROR_TYPE           0  /*��������                                              */

/*Hex������غ�������*/
extern uint8_t  AsciiToDec(uint8_t *p_AsciiData);
extern uint8_t  GetHexLineInfo(uint8_t *p_HexData, t_HexLineInfo *p_HexDataInfo);
extern uint8_t  GetHexData(t_HexLineInfo *p_HexDataInfo, uint8_t *p_HexData);

#endif  /*__DECODEHEX_H*/
