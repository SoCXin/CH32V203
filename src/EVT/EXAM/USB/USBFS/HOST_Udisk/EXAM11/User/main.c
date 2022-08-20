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
    UINT8  flg;
    UINT8  ret, s;
    UINT8  i;
    UINT8  *pCodeStr;
    UINT16 j;


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
                flg = 1;
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
                        /* ��ȡԭ�ļ� */
                        printf( "Open\r\n" );
                        strcpy( mCmdParam.Open.mPathName, "/C51/CHRV3HFT.C" );//�ļ���,���ļ���C51��Ŀ¼��
                        s = CHRV3FileOpen( );                        //���ļ�
                        /* �г��ļ� */
                        if ( s == ERR_MISS_DIR )
                        {
                            printf("�����ڸ��ļ����г������ļ�\r\n");  //C51��Ŀ¼���������г���Ŀ¼�µ������ļ�
                            pCodeStr = "/*";
                        }
                        else
                        {
                            pCodeStr = "/C51/*";                     //* CHRV3HFT.C�ļ����������г�\C51��Ŀ¼�µ���CHRV3��ͷ���ļ�
                        }
                        printf( "List file %s\r\n", pCodeStr );
                        for ( j = 0; j < 10000; j ++ )               //�������ǰ10000���ļ�,ʵ����û������
                        {
                            strcpy( mCmdParam.Open.mPathName, pCodeStr );//�����ļ���,*Ϊͨ���,�����������ļ�������Ŀ¼
                            i = strlen( mCmdParam.Open.mPathName );
                            mCmdParam.Open.mPathName[ i ] = 0xFF;    //�����ַ������Ƚ��������滻Ϊ���������,��0��254,�����0xFF��255��˵�����������CHRV3vFileSize������
                            CHRV3vFileSize = j;                      //ָ������/ö�ٵ����
                            i = CHRV3FileOpen( );                    //���ļ�,����ļ����к���ͨ���*,��Ϊ�����ļ�������
                            /* CHRV3FileEnum �� CHRV3FileOpen ��Ψһ�����ǵ����߷���ERR_FOUND_NAMEʱ��ô��Ӧ��ǰ�߷���ERR_SUCCESS */
                            if ( i == ERR_MISS_FILE )
                            {
                                break;                                //��Ҳ��������ƥ����ļ�,�Ѿ�û��ƥ����ļ���
                            }
                            if ( i == ERR_FOUND_NAME )
                            {
                                /* ��������ͨ�����ƥ����ļ���,�ļ�����������·������������� */
                                printf( "  match file %04d#: %s\r\n", (unsigned int)j, mCmdParam.Open.mPathName );//��ʾ��ź���������ƥ���ļ���������Ŀ¼��
                                continue;                             //����������һ��ƥ����ļ���,�´�����ʱ��Ż��1
                            }
                            else
                            {
                                /* ���� */
                                mStopIfError( i );
                                break;
                            }
                        }
                        printf( "Close\r\n" );
                        i = CHRV3FileClose( );                         //�ر��ļ�
                        printf( "U����ʾ���\r\n" );
                        break;
                    }
                    else
                    {
                        printf("s = %02x\r\n",s);
                        printf("CHRV3DiskStatus:%02x\r\n",CHRV3DiskStatus);
                    }
                }
            }
            else
            {
                printf( "Device Enum Failed\r\n" );
            }
        }
    }
}

