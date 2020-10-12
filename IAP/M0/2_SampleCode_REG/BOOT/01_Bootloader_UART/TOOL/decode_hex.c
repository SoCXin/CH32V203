#include    "decode_hex.h"
/*-----------------------------------------------------------------------------------------------*/

/*----------------------------------------- ȫ�ֱ��� --------------------------------------------*/
extern uint32_t g_AddrBase;
extern uint32_t g_UserProgramAddr;//�û�����ִ�е�ַ
extern uint32_t g_HaveGetAddr;//��ȡ�û�����ִ�е�ַ��־�����Ѿ���ȡ��Ϊ1
/*-----------------------------------------------------------------------------------------------*/

/*-------------------------------------- �Զ����������� -----------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/

//*================================================================================================
//*                                         ��  ��  ��
//*================================================================================================

/**************************************************************************************************
** �������� : AsciiToDec
** �������� : ��һ��ASCII�ַ���ת��Ϊ��ֵ
** ��ڲ��� : <p_AsciiData>[in] ��ת����ASCII���ݵ�ַ
** ���ڲ��� : ��
** �� �� ֵ : ת�������ֵ
** ����˵�� : ��
***************************************************************************************************/
uint8_t AsciiToDec(uint8_t *p_AsciiData)
{
    if (p_AsciiData != NULL)
    {
        if ((*p_AsciiData >= '0') && (*p_AsciiData <= '9'))
        {
            return ((*p_AsciiData) - '0');
        }
        else if ((*p_AsciiData >= 'A') && (*p_AsciiData <= 'F'))
        {
            return ((*p_AsciiData) - 'A' + 10);
        }
        else if ((*p_AsciiData >= 'a') && (*p_AsciiData <= 'f'))
        {
            return ((*p_AsciiData) - 'a' + 10);
        }
        else
        {
            return (*p_AsciiData);
        }
    }
    else
    {
        return ERROR;
    }
}
/**************************************************************************************************
** �������� : GetHexLineInfo
** �������� : ��ȡHex�ļ�һ�����ݵ���Ϣ�����������t_HexLineInfo���ͽṹ�������
** ��ڲ��� : <p_HexData>[in] Hex������ʼ���ݵ�ַ
** ���ڲ��� : <p_HexDataInfo>[out] һ��Hex������Ϣ
** �� �� ֵ : �������쳣����0
** ����˵�� : ��
***************************************************************************************************/
uint8_t GetHexLineInfo(uint8_t *p_HexData, t_HexLineInfo *p_HexDataInfo)
{
    uint8_t i = 0;
    uint8_t cc = 0;
    uint8_t ErrorCode = 0;
    if ((p_HexData != NULL) && (p_HexDataInfo != NULL))
    {
        if ((*p_HexData) != ':') //�ж������Ƿ��ԡ�:����ͷ
        {
            ErrorCode = 1;
            return ErrorCode;
        }
        else //�ж�����У���Ƿ���ȷ
        {
            for (i = 0; i < (AsciiToDec(p_HexData + 1) << 4) + AsciiToDec(p_HexData + 2) + 4; i++)
            {
                cc += (AsciiToDec(p_HexData + 1 + i * 2) << 4) + AsciiToDec(p_HexData + 2 + i * 2);
            }
            cc = ~(cc % 0x100) + 1;
        }
        p_HexDataInfo->SumCheckOk = FALSE;
        if (cc == (AsciiToDec(p_HexData + i * 2 + 1) << 4) + AsciiToDec(p_HexData + i * 2 + 2)) //����У����ȷ
        {
            switch (*(p_HexData + 8))
            {
            case RCD_DATA:
                p_HexDataInfo->DataLength = (AsciiToDec(p_HexData + 1) << 4) + AsciiToDec(p_HexData + 2);
                p_HexDataInfo->OffsetAddr = (AsciiToDec(p_HexData + 3) << 12) + (AsciiToDec(p_HexData + 4) << 8) +
                                            (AsciiToDec(p_HexData + 5) << 4) + (AsciiToDec(p_HexData + 6) << 0);
                p_HexDataInfo->Rectype = *(p_HexData + 8);
                p_HexDataInfo->p_Data = p_HexData + 9;
                p_HexDataInfo->SumCheckOk = TRUE;
                ErrorCode = 0;
                if (g_HaveGetAddr == 0)
                {
                    g_UserProgramAddr = (g_AddrBase << 16) + p_HexDataInfo->OffsetAddr;
                    g_HaveGetAddr = 1;
                }
                break;
            case RCD_END_FILE:
                p_HexDataInfo->DataLength = (AsciiToDec(p_HexData + 1) << 4) + AsciiToDec(p_HexData + 2);
                p_HexDataInfo->Rectype = *(p_HexData + 8);
                p_HexDataInfo->SumCheckOk = TRUE;
                ErrorCode = 0;
                break;
            case RCD_EXT_SEGMENT_ADDR:
                p_HexDataInfo->Rectype = *(p_HexData + 8);
                p_HexDataInfo->SumCheckOk = TRUE;
                ErrorCode = 0;
                break;
            case RCD_START_SEGMENT_ADDR:
                p_HexDataInfo->Rectype = *(p_HexData + 8);
                p_HexDataInfo->SumCheckOk = TRUE;
                ErrorCode = 0;
                break;
            case RCD_EXT_LINEAR_ADDR:
                p_HexDataInfo->Rectype = *(p_HexData + 8);
                p_HexDataInfo->DataLength = (AsciiToDec(p_HexData + 1) << 4) + AsciiToDec(p_HexData + 2);
                //p_HexDataInfo->DataLength=0;
                p_HexDataInfo->AddrBase = (AsciiToDec(p_HexData + 9) << 12) + (AsciiToDec(p_HexData + 10) << 8) +
                                          (AsciiToDec(p_HexData + 11) << 4) + (AsciiToDec(p_HexData + 12) << 0);
                g_AddrBase = p_HexDataInfo->AddrBase;
                p_HexDataInfo->SumCheckOk = TRUE;
                ErrorCode = 0;
                break;
            case RCD_START_LINEAR_ADDR:
                p_HexDataInfo->Rectype = *(p_HexData + 8);
                p_HexDataInfo->DataLength = (AsciiToDec(p_HexData + 1) << 4) + AsciiToDec(p_HexData + 2);
                p_HexDataInfo->SumCheckOk = TRUE;
                ErrorCode = 0;
                break;
            default:
                p_HexDataInfo->Rectype = RCD_ERROR_TYPE;
                p_HexDataInfo->SumCheckOk = TRUE;
                ErrorCode = 2;
                break;
            }
        }
        else
        {
            ErrorCode = 3;
        }
    }
    else
    {
        ErrorCode = 4;
    }
    return ErrorCode;
}
/**************************************************************************************************
** �������� : GetHexData
** �������� : ���ݴ����p_HexDataInfoָ�������ȡHex�ļ�һ�е��������ݣ��������ݴ���p_HexData������
** ��ڲ��� : <p_HexData>[in] Hex������ʼ���ݵ�ַ
** ���ڲ��� : <p_HexDataInfo>[out] һ��Hex������Ϣ
** �� �� ֵ : �������쳣����0
** ����˵�� : ��
***************************************************************************************************/
uint8_t GetHexData(t_HexLineInfo *p_HexDataInfo, uint8_t *p_HexData)
{
    uint8_t ErrorCode = 0;
    uint8_t i = 0;
    if ((p_HexDataInfo != NULL) && (p_HexData != NULL))
    {
        switch (p_HexDataInfo->Rectype)
        {
        case RCD_DATA:
            for (i = 0; i < p_HexDataInfo->DataLength; i++)
            {
                p_HexData[i] = (AsciiToDec(p_HexDataInfo->p_Data + 2 * i) << 4) + (AsciiToDec(p_HexDataInfo->p_Data + 2 * i + 1));
            }
            break;
        case RCD_END_FILE:
            break;
        case RCD_EXT_SEGMENT_ADDR:
            break;
        case RCD_START_SEGMENT_ADDR:
            break;
        case RCD_EXT_LINEAR_ADDR:
            break;
        case RCD_START_LINEAR_ADDR:
            break;
        default:
            ErrorCode = 1;
            break;
        }
    }
    else
    {
        ErrorCode = 2;
    }
    return ErrorCode;
}

