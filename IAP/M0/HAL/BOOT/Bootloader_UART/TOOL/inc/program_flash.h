//*================================================================================================
//* 文件名              : ProgramFlash.h
//* 功能描述            : 将解析后的Hex数据烧写在Flash中的应用层函数
//* 作者                : 罗伟东
//* 版本                : 0.01
//* 建立日期、时间      : 2011/04/15 19:50
//* 最近修改日期、时间  :
//* 修改原因            :
//*================================================================================================
#ifndef __PROGRAMFLASH_H
#define __PROGRAMFLASH_H

#include "main.h"
#include <stdio.h>

#define USE_DFT_ADDR                0x01    /*使用HEX文件中的默认地址值                         */
#define USE_USER_DEFINED_ADDR       0x02    /*使用自己指定的地址值                              */

#define BUFFER_SIZE                 8*1024    /*暂存HEX文件数据缓冲区大小        256U       2048U          */

extern uint8_t ProgramHexToFlash(uint8_t *p_HexData, uint32_t AddrSelect, volatile uint32_t UserAdd);
extern uint8_t EraseFlash(uint32_t StartSector, uint32_t EndSector);
extern uint8_t DetectSector(uint32_t Address);

#endif /*__PROGRAMFLASH_H*/
