/********************************** (C) COPYRIGHT *******************************
* File Name          : RingMem.C
* Author             : Hikari
* Version            : V1.4
* Date               : 2019/12/20
* Description        : ���ʹ���ж���ע�⿪���ж�
            
*******************************************************************************/

/*********************************************************************
 * INCLUDES
 */
#include "RingMem.h"

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * PUBLIC FUNCTIONS
 */
RingMemProtection_t RingMemProtection;
/*********************************************************************
 * @fn      RingMemInit
 *
 * @brief   ��ʼ��
 *
 * @param   none
 *
 * @return  none
 */
void RingMemInit( RingMemParm_t *Parm, uint8_t *StartAddr, uint32_t MaxLen, RingMemProtection_t Protection )
{
	Parm->pData		= StartAddr;
	Parm->pWrite 	= StartAddr;
	Parm->pRead  	= StartAddr;
	Parm->RemanentLen = MaxLen;
	Parm->CurrentLen 	= 0;
	Parm->pEnd	  = StartAddr + MaxLen;
	Parm->MaxLen  = MaxLen;
	RingMemProtection = Protection;
}

/*********************************************************************
 * @fn      RingMemWrite
 *
 * @brief   ����д
 *
 * @param   none
 *
 * @return  none
 */
uint8_t RingMemWrite( RingMemParm_t *Parm, uint8_t *pData, uint32_t len )
{
	uint32_t i,edgelen;
	
	if(RingMemProtection)
	{
	    RingMemProtection(ENABLE);
	}

	if( len > Parm->RemanentLen )							//Ҫд���ȳ���
		return !SUCCESS;
	
	edgelen = Parm->pEnd - Parm->pWrite;			//����ʣ�ൽ�߽�ĳ���
	
	if( len > edgelen )
	{
		for(i=0; i<edgelen; i++)
		{
			*Parm->pWrite++ = *pData++;		
		}
		Parm->pWrite = Parm->pData;	
		for(i=0; i<(len-edgelen); i++)
		{
			*Parm->pWrite++ = *pData++;		
		}		
	}
	else
	{
		for(i=0; i<len; i++)
		{
			*Parm->pWrite++ = *pData++;
		}
	}
	
	Parm->RemanentLen -= len;	
	Parm->CurrentLen 	+= len;	

    if(RingMemProtection)
    {
        RingMemProtection(DISABLE);
    }

	return SUCCESS;
}

/*********************************************************************
 * @fn      RingMemRead
 *
 * @brief   �����
 *
 * @param   none
 *
 * @return  none
 */
uint8_t RingMemRead( RingMemParm_t *Parm, uint8_t *pData, uint32_t len )
{
	uint32_t i,edgelen;
	
    if(RingMemProtection)
    {
        RingMemProtection(ENABLE);
    }

    if( len > Parm->CurrentLen )						//�ܶ����Ȳ���
		return !SUCCESS;
	
	edgelen = Parm->pEnd - Parm->pRead;			//����ʣ�ൽ�߽�ĳ���
	
	if( len > edgelen )
	{
		for(i=0; i<edgelen; i++)
		{
			*pData++ = *Parm->pRead++;		
		}
		Parm->pRead = Parm->pData;	
		for(i=0; i<(len-edgelen); i++)
		{
			*pData++ = *Parm->pRead++;		
		}
	}
	else
	{
		for(i=0; i<len; i++)
		{
			*pData++ = *Parm->pRead++;		
		}
	}
	
	Parm->RemanentLen += len;
	Parm->CurrentLen 	-= len;	

	if(RingMemProtection)
    {
        RingMemProtection(DISABLE);
    }

	return SUCCESS;
}

/*********************************************************************
 * @fn      RingMemCopy
 *
 * @brief   ���˲�ɾ
 *
 * @param   none
 *
 * @return  none
 */
uint8_t RingMemCopy( RingMemParm_t *Parm, uint8_t *pData, uint32_t len )
{
	uint32_t i,edgelen;

    if(RingMemProtection)
    {
        RingMemProtection(ENABLE);
    }

	uint8_t volatile *pRead = Parm->pRead;
	
	if( len > Parm->CurrentLen )						//�ܸ��Ƶĳ��Ȳ���
		return !SUCCESS;
	
	edgelen = Parm->pEnd - Parm->pRead;			//����ʣ�ൽ�߽�ĳ���
	
	if( len > edgelen )
	{
		for(i=0; i<edgelen; i++)
		{
			*pData++ = *pRead++;		
		}
		pRead = Parm->pData;	
		for(i=0; i<(len-edgelen); i++)
		{
			*pData++ = *pRead++;		
		}
	}
	else
	{
		for(i=0; i<len; i++)
		{
			*pData++ = *pRead++;		
		}
	}
	
    if(RingMemProtection)
    {
        RingMemProtection(DISABLE);
    }

	return SUCCESS;
}

/*********************************************************************
 * @fn      RingMemDelete
 *
 * @brief   ֱ��ɾ
 *
 * @param   none
 *
 * @return  none
 */
uint8_t RingMemDelete( RingMemParm_t *Parm, uint32_t len )
{
	uint32_t edgelen;
	
    if(RingMemProtection)
    {
        RingMemProtection(ENABLE);
    }

	if( len > Parm->CurrentLen )						//��ɾ�����Ȳ���
		return !SUCCESS;
	
	edgelen = Parm->pEnd - Parm->pRead;			//����ʣ�ൽ�߽�ĳ���
	
	if( len > edgelen )
	{
		Parm->pRead+=edgelen;		
		Parm->pRead = Parm->pData;	
		Parm->pRead+=(len-edgelen);		
	}
	else
	{
		Parm->pRead+=len;		
	}
	
	Parm->RemanentLen += len;
	Parm->CurrentLen 	-= len;	

    if(RingMemProtection)
    {
        RingMemProtection(DISABLE);
    }

    return SUCCESS;
}

/*********************************************************************
 * @fn      RingAddInStart
 *
 * @brief   �򻺳�����ʼ���������
 *
 * @param   none
 *
 * @return  none
 */
uint8_t RingAddInStart( RingMemParm_t *Parm, uint8_t *pData, uint32_t len )
{
	uint32_t i,edgelen;
	
    if(RingMemProtection)
    {
        RingMemProtection(ENABLE);
    }

	if( len > Parm->RemanentLen )							//Ҫд���ȵĳ���
		return !SUCCESS;
	
	edgelen = Parm->pRead - Parm->pData;			//����ʣ�ൽ�߽�ĳ���
	
	if( len > edgelen )
	{
		for(i=0; i<edgelen; i++)
		{
			*Parm->pRead-- = *pData++;		
		}
		Parm->pRead = Parm->pData + Parm->MaxLen;	
		for(i=0; i<(len-edgelen); i++)
		{
			*Parm->pRead-- = *pData++;		
		}
	}
	else
	{
		for(i=0; i<len; i++)
		{
			*Parm->pRead-- = *pData++;
		}
	}
	
	Parm->RemanentLen -= len;
	Parm->CurrentLen 	+= len;	

	if(RingMemProtection)
    {
        RingMemProtection(DISABLE);
    }

	return SUCCESS;
}

/*********************************************************************
 * @fn      RingCopyOneData
 *
 * @brief   ���׵�ַΪ�������صڼ�������
 *
 * @param   none
 *
 * @return  none
 */
uint8_t RingReturnSingleData( RingMemParm_t *Parm, uint32_t Num )
{
  uint32_t edgelen;
  
  if( Parm->RemanentLen < Parm->MaxLen )
  {    
    edgelen = Parm->pEnd - Parm->pRead;        
    if( Num >= edgelen )
    {
      return Parm->pData[Num - edgelen];
    }
    else
    {
      return Parm->pRead[Num];
    }
  }
  else
    return 0;
}
/*********************************************************************
*********************************************************************/
