//*================================================================================================
//* 文件名              : decodeHex.h
//* 功能描述            : 解析Hex文件的相关函数声明及数据定义
//* 作者                : 罗伟东
//* 版本                : 0.01
//* 建立日期、时间      : 2011/04/13 21:27
//* 最近修改日期、时间  :
//* 修改原因            :
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
    uint32_t    DataLength;                 /*是这条记录的长度域，他表示数据(dd)的字节数目          */
    uint32_t    AddrBase;                   /*基地址信息                                            */
    uint32_t    OffsetAddr;                 /*偏移地址信息                                          */
    uint32_t    Rectype;                    /*这个域表示这条HEX记录的类型                           */
    uint32_t    SumCheckOk;                 /*数据和校验正确                                        */
    uint8_t *p_Data;                    /*数据域指针                                            */
    //uint8_t   *p_NextLineAddr;            /*下一行hex数据指针                                     */
} t_HexLineInfo;

/*Rectype数据类型取值*/
#define     RCD_DATA                '0' /*用来记录数据，HEX文件的大部分记录都是数据记录         */
#define     RCD_END_FILE            '1' /*用来标识文件结束，放在文件的最后，标识HEX文件的结尾   */
#define     RCD_EXT_SEGMENT_ADDR    '2' /*用来标识扩展段地址的记录                              */
#define     RCD_START_SEGMENT_ADDR  '3' /*用来标识起始段地址的记录                              */
#define     RCD_EXT_LINEAR_ADDR     '4' /*用来标识扩展线性地址的记录                            */
#define     RCD_START_LINEAR_ADDR   '5' /*用来标识起始线性地址的记录                            */
#define     RCD_ERROR_TYPE           0  /*错误类型                                              */

/*Hex译码相关函数声明*/
extern uint8_t  AsciiToDec(uint8_t *p_AsciiData);
extern uint8_t  GetHexLineInfo(uint8_t *p_HexData, t_HexLineInfo *p_HexDataInfo);
extern uint8_t  GetHexData(t_HexLineInfo *p_HexDataInfo, uint8_t *p_HexData);

#endif  /*__DECODEHEX_H*/
