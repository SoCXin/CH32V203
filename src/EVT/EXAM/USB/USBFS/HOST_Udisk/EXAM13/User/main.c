/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2021/06/06
 * Description        : Main program body.
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * SPDX-License-Identifier: Apache-2.0
 *******************************************************************************/

/*
 *@Note
  USBFS�豸�ļ���ö�ٹ������̣�
  OTG_FS_DM(PA11)��OTG_FS_DP(PA12)��

*/
#include "debug.h"
#include "stdio.h"
#include "string.h"
#include "ch32vf20x_usbotg_host.h"
#include "UDisk_LongName.h"
#include "CHRV3UFI.h"

__attribute__ ((aligned(4))) UINT8   buf[ 128 ];  // OUT, must even address
__attribute__ ((aligned(4))) UINT8   MY_DATA_BUF[ DISK_BASE_BUF_LEN ];   /* MY_DATA_BUFָ���ⲿRAM�Ĵ������ݻ�����,����������Ϊ����һ�������ĳ���,�����û����ݻ���*/

/* ������״̬,�����������ʾ������벢ͣ�� */
void mStopIfError( UINT8 iError )
{
    if ( iError == ERR_SUCCESS )
    {
        return;    /* �����ɹ� */
    }
    printf( "Error:%02x\r\n", iError );  /* ��ʾ���� */
    /* ���������,Ӧ�÷����������Լ�CHRV3DiskStatus״̬,�������CHRV3DiskReady��ѯ��ǰU���Ƿ�����,���U���ѶϿ���ô�����µȴ�U�̲����ٲ���,
         ��������Ĵ�����:
         1������һ��CHRV3DiskReady,�ɹ����������,����Open,Read/Write��
         2�����CHRV3DiskReady���ɹ�,��ôǿ�н���ͷ��ʼ���� */
    while(1)
    {  }
}

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{
    UINT8  ret, s;
    UINT8  i;
    UINT16 j;
    UINT8  len;

    USART_Printf_Init(115200);
    Delay_Init( );
    printf("SystemClk:%d\r\n",SystemCoreClock);
    printf("USBFS HOST Test EXAM 11\r\n");

    USBOTG_HostInit(ENABLE);

    ret = CHRV3LibInit();
    if( ret == ERR_SUCCESS )
    {
        printf( "UDisk Lib Init Success\n" );
    }
    else
    {
        printf( "UDisk Lib Init ERR Code %02x\n", ret );
        while(1);
    }

    while(1)
    {
        s = ERR_SUCCESS;
        if ( USBOTG_FS->INT_FG & USBHD_UIF_DETECT )
        {
            USBOTG_FS->INT_FG = USBHD_UIF_DETECT ;

            s = AnalyzeRootHub( );
            if ( s == ERR_USB_CONNECT )
            {
                printf( "New Device In\r\n" );
                FoundNewDev = 1;
            }
            if( s == ERR_USB_DISCON )
            {
                printf( "Device Out\r\n" );
            }
        }

        if ( FoundNewDev || s == ERR_USB_CONNECT )
        {
            FoundNewDev = 0;
            Delay_Ms( 200 );
            s = USBOTG_HostEnum( buf );
            if ( s == ERR_SUCCESS )
            {
                printf( "Enum Succeed\r\n" );
                Delay_Ms(100);
                // U�̲������̣�USB���߸�λ��U�����ӡ���ȡ�豸������������USB��ַ����ѡ�Ļ�ȡ������������֮�󵽴�˴�����CH103�ӳ���������ɺ�������
                CHRV3DiskStatus = DISK_USB_ADDR;
                for ( i = 0; i != 10; i ++ )
                {
                    printf( "Wait DiskReady\r\n" );
                    s = CHRV3DiskReady( );                                 //�ȴ�U��׼����
                    if ( s == ERR_SUCCESS )
                    {
                        /*==================== ������ʾ��������ȡ���ļ��� ============================*/
                        // ���Ƴ��ļ���(UNICODE ���)��LongNameBuf��
                        len = LongName_Len;
                        memcpy( LongNameBuf, LongName, len );
                        // ĩβ������0��ʾ����
                        LongNameBuf[len] = 0x00;
                        LongNameBuf[len + 1] = 0x00;
                        // �ó��ļ�����ANSI������ļ���(8+3��ʽ)
                        strcpy( mCmdParam.Create.mPathName, "\\���ļ���.TXT" );
                        i = CHRV3CreateLongName( );
                        if( i == ERR_SUCCESS )
                            printf( "Created Long Name OK!\r\n" );
                        else
                            printf( "Error Code: %02X\r\n", (UINT16)i );

                        printf( "Get long Name#\r\n" );
                        strcpy( mCmdParam.Open.mPathName, "\\���ļ���.TXT" );
                        // ������Ҫ�����ļ���������·��
                        i = CHRV3GetLongName( );
                        if( i == ERR_SUCCESS )
                        {
                            // ���ļ����ռ����,��UNICODE���뷽ʽ(��UNICODE_ENDIAN����)
                            // �����LongNameBuf������,���ļ������������0����.
                            // ������ʾ����������������
                            printf( "LongNameBuf: " );
                            for( j=0; j!=LONG_NAME_BUF_LEN; j++ )
                                printf( "%02X ", (UINT16)LongNameBuf[j] );
                            printf( "\r\n" );
                        }
                        else
                            printf( "Error Code: %02X\r\n", (UINT16)i );
                        break;
                    }
                    else
                    {
                        printf("s = %02x\r\n",s);
                        printf("CHRV3DiskStatus:%02x\r\n",CHRV3DiskStatus);
                    }
//                    Delay_Ms( 50 );
                }
            }
            else
            {
                printf( "Enum Failed:%02x\r\n", ret );
            }
        }
    }
}

