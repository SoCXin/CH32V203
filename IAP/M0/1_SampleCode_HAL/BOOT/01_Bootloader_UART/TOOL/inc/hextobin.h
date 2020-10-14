#ifndef __HEXTOBIN_H_
#define __HEXTOBIN_H_
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef struct{
	int len; //bin�ļ���С
	uint32_t startAddress; //ˢд����ʼ��ַ
	uint8_t *pContent;		//ת���������
}HexToBinData;
 
typedef struct{
	uint8_t data[16];//����
	uint8_t len;	//���ݳ���
	uint8_t pos;	//ƫ�Ƶ�ַ
	uint8_t type;	//����
}HexLinData;
 
int ConvertHexToBin(const char *str,HexToBinData *pData);
#endif