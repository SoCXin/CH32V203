//*================================================================================================
//* �ļ���              : ProgramFlash.h
//* ��������            : ���������Hex������д��Flash�е�Ӧ�ò㺯��
//* ����                : ��ΰ��
//* �汾                : 0.01
//* �������ڡ�ʱ��      : 2011/04/15 19:50
//* ����޸����ڡ�ʱ��  :
//* �޸�ԭ��            :
//*================================================================================================
#ifndef __PROGRAMFLASH_H
#define __PROGRAMFLASH_H

#include "main.h"
#include <stdio.h>

#define USE_DFT_ADDR                0x01    /*ʹ��HEX�ļ��е�Ĭ�ϵ�ֵַ                         */
#define USE_USER_DEFINED_ADDR       0x02    /*ʹ���Լ�ָ���ĵ�ֵַ                              */

#define BUFFER_SIZE                 8*1024    /*�ݴ�HEX�ļ����ݻ�������С        256U       2048U          */

extern uint8_t ProgramHexToFlash(uint8_t *p_HexData, uint32_t AddrSelect, volatile uint32_t UserAdd);
extern uint8_t EraseFlash(uint32_t StartSector, uint32_t EndSector);
extern uint8_t DetectSector(uint32_t Address);

#endif /*__PROGRAMFLASH_H*/
