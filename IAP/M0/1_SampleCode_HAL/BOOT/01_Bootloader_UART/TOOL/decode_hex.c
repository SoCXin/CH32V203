#include    "decode_hex.h"
/*-----------------------------------------------------------------------------------------------*/

/*----------------------------------------- 全局变量 --------------------------------------------*/
extern uint32_t g_AddrBase;
extern uint32_t g_UserProgramAddr;//用户程序执行地址
extern uint32_t g_HaveGetAddr;//获取用户程序执行地址标志，若已经获取则为1
/*-----------------------------------------------------------------------------------------------*/

/*-------------------------------------- 自定义数据类型 -----------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/

//*================================================================================================
//*                                         函  数  区
//*================================================================================================

/**************************************************************************************************
** 函数名称 : AsciiToDec
** 功能描述 : 将一个ASCII字符码转换为数值
** 入口参数 : <p_AsciiData>[in] 被转换的ASCII数据地址
** 出口参数 : 无
** 返 回 值 : 转换后的数值
** 其他说明 : 无
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
** 函数名称 : GetHexLineInfo
** 功能描述 : 获取Hex文件一行数据的信息，并将其存入t_HexLineInfo类型结构体变量中
** 入口参数 : <p_HexData>[in] Hex数据起始数据地址
** 出口参数 : <p_HexDataInfo>[out] 一行Hex数据信息
** 返 回 值 : 无其他异常返回0
** 其他说明 : 无
***************************************************************************************************/
uint8_t GetHexLineInfo(uint8_t *p_HexData, t_HexLineInfo *p_HexDataInfo)
{
    uint8_t i = 0;
    uint8_t cc = 0;
    uint8_t ErrorCode = 0;
    if ((p_HexData != NULL) && (p_HexDataInfo != NULL))
    {
        if ((*p_HexData) != ':') //判断数据是否以‘:’开头
        {
            ErrorCode = 1;
            return ErrorCode;
        }
        else //判断数据校验是否正确
        {
            for (i = 0; i < (AsciiToDec(p_HexData + 1) << 4) + AsciiToDec(p_HexData + 2) + 4; i++)
            {
                cc += (AsciiToDec(p_HexData + 1 + i * 2) << 4) + AsciiToDec(p_HexData + 2 + i * 2);
            }
            cc = ~(cc % 0x100) + 1;
        }
        p_HexDataInfo->SumCheckOk = FALSE;
        if (cc == (AsciiToDec(p_HexData + i * 2 + 1) << 4) + AsciiToDec(p_HexData + i * 2 + 2)) //数据校验正确
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
** 函数名称 : GetHexData
** 功能描述 : 根据传入的p_HexDataInfo指针变量获取Hex文件一行的有用数据，并将数据存入p_HexData数组中
** 入口参数 : <p_HexData>[in] Hex数据起始数据地址
** 出口参数 : <p_HexDataInfo>[out] 一行Hex数据信息
** 返 回 值 : 无其他异常返回0
** 其他说明 : 无
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

