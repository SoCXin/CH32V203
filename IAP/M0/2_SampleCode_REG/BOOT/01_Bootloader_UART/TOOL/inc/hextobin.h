#ifndef __HEXTOBIN_H_
#define __HEXTOBIN_H_
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef struct{
	int len; //bin文件大小
	uint32_t startAddress; //刷写的起始地址
	uint8_t *pContent;		//转化后的内容
}HexToBinData;
 
typedef struct{
	uint8_t data[16];//数据
	uint8_t len;	//数据长度
	uint8_t pos;	//偏移地址
	uint8_t type;	//类型
}HexLinData;
 
int ConvertHexToBin(const char *str,HexToBinData *pData);
#endif