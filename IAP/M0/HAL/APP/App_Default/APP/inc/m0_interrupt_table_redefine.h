/******************************************************************************
 * @file    m0_int_tab_redefine.h
 * @author  MM32 AE
 * @version V1.00
 * @date    10-June-2020
 * @brief   ......
 ******************************************************************************
 *  @attention
 *
 *  THE EXISTING FIRMWARE IS ONLY FOR REFERENCE, WHICH IS DESIGNED TO PROVIDE
 *  CUSTOMERS WITH CODING INFORMATION ABOUT THEIR PRODUCTS SO THEY CAN SAVE
 *  TIME. THEREFORE, MINDMOTION SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT OR
 *  CONSEQUENTIAL DAMAGES ABOUT ANY CLAIMS ARISING OUT OF THE CONTENT OF SUCH
 *  HARDWARE AND/OR THE USE OF THE CODING INFORMATION CONTAINED HEREIN IN
 *  CONNECTION WITH PRODUCTS MADE BY CUSTOMERS.
 *  <H2><CENTER>&COPY; COPYRIGHT 2020 MINDMOTION </CENTER></H2>
******************************************************************************/

/* Define to prevent recursive inclusion ------------------------------------*/
#ifndef _M0_INT_TAB_REDEFINE_H__
#define _M0_INT_TAB_REDEFINE_H__


#ifdef __cplusplus
extern "C" {
#endif

#undef  EXTERN


#ifdef  __MAIN_C__
#define EXTERN
#else
#define EXTERN extern
#endif

/* Includes -----------------------------------------------------------------*/


/* Includes -----------------------------------------------------------------*/


/* Exported constants -------------------------------------------------------*/

#if defined (MM32F032_S) || defined (MM32L06XX_S)
typedef struct
{
    /*   中断处理函数指针表，在中断服务函数中调用函数指针 Total NUM = 0x30,48D  */
    void (*pInitial_SP);    /*   Top of Stack   */
    void (*pReset_Handler)(void);
    void (*pNMI_Handler)(void);
    void (*pHardFault_Handler)(void);
    void (*pReserved0_Handler)(void);
    void (*pReserved1_Handler)(void);
    void (*pReserved2_Handler)(void);
    void (*pReserved3_Handler)(void);
    void (*pReserved4_Handler)(void);
    void (*pReserved5_Handler)(void);
    void (*pReserved6_Handler)(void);
    void (*pSVC_Handler)(void);
    void (*pReserved7_Handler)(void);
    void (*pReserved8_Handler)(void);
    void (*pPendSV_Handler)(void);
    void (*pSysTick_Handler)(void);
    void (*pWWDG_IRQHandler)(void);     /*   External Interrupts    */
    void (*pPVD_IRQHandler)(void);
    void (*pRTC_BKP_IRQHandler)(void);
    void (*pFLASH_IRQHandler)(void);
    void (*pRCC_CRS_IRQHandler)(void);
    void (*pEXTI0_1_IRQHandler)(void);
    void (*pEXTI2_3_IRQHandler)(void);
    void (*pEXTI4_15_IRQHandler)(void);
    void (*pReserved0_IRQHandler)(void);
    void (*pDMA1_Channel1_IRQHandler)(void);
    void (*pDMA1_Channel2_3_IRQHandler)(void);
    void (*pDMA1_Channel4_5_IRQHandler)(void);
    void (*pADC1_COMP_IRQHandler)(void);
    void (*pTIM1_BRK_UP_TRG_COM_IRQHandler)(void);
    void (*pTIM1_CC_IRQHandler)(void);
    void (*pTIM2_IRQHandler)(void);
    void (*pTIM3_IRQHandler)(void);
    void (*pReserved1_IRQHandler)(void);
    void (*pReserved2_IRQHandler)(void);
    void (*pTIM14_IRQHandler)(void);
    void (*pReserved3_IRQHandler)(void);
    void (*pTIM16_IRQHandler)(void);
    void (*pTIM17_IRQHandler)(void);
    void (*pI2C1_IRQHandler)(void);
    void (*pReserved4_IRQHandler)(void);
    void (*pSPI1_IRQHandler)(void);
    void (*pSPI2_IRQHandler)(void);
    void (*pUART1_IRQHandler)(void);
    void (*pUART2_IRQHandler)(void);
    void (*pReserved5_IRQHandler)(void);
    void (*pCAN_IRQHandler)(void);
    void (*pUSB_IRQHandler)(void);
}NVIC_TABLE_t;
#elif (MM32SPIN06XX_S)
typedef struct
{
    /*   中断处理函数指针表，在中断服务函数中调用函数指针  Total NUM = 0x30,48D  */
    void (*pInitial_SP);    /*   Top of Stack   */
    void (*pReset_Handler)(void);
    void (*pNMI_Handler)(void);
    void (*pHardFault_Handler)(void);
    void (*pReserved0_Handler)(void);
    void (*pReserved1_Handler)(void);
    void (*pReserved2_Handler)(void);
    void (*pReserved3_Handler)(void);
    void (*pReserved4_Handler)(void);
    void (*pReserved5_Handler)(void);
    void (*pReserved6_Handler)(void);
    void (*pSVC_Handler)(void);
    void (*pReserved7_Handler)(void);
    void (*pReserved8_Handler)(void);
    void (*pPendSV_Handler)(void);
    void (*pSysTick_Handler)(void);
    void (*pWWDG_IRQHandler)(void);     /*   External Interrupts    */
    void (*pPVD_IRQHandler)(void);
    void (*pRTC_BKP_IRQHandler)(void);
    void (*pFLASH_IRQHandler)(void);
    void (*pRCC_CRS_IRQHandler)(void);
    void (*pEXTI0_1_IRQHandler)(void);
    void (*pEXTI2_3_IRQHandler)(void);
    void (*pEXTI4_15_IRQHandler)(void);
    void (*pReserved0_IRQHandler)(void);
    void (*pDMA1_Channel1_IRQHandler)(void);
    void (*pDMA1_Channel2_3_IRQHandler)(void);
    void (*pDMA1_Channel4_5_IRQHandler)(void);
    void (*pADC1_COMP_IRQHandler)(void);
    void (*pTIM1_BRK_UP_TRG_COM_IRQHandler)(void);
    void (*pTIM1_CC_IRQHandler)(void);
    void (*pTIM2_IRQHandler)(void);
    void (*pTIM3_IRQHandler)(void);
    void (*pReserved1_IRQHandler)(void);
    void (*pReserved2_IRQHandler)(void);
    void (*pTIM14_IRQHandler)(void);
    void (*pReserved3_IRQHandler)(void);
    void (*pTIM16_IRQHandler)(void);
    void (*pTIM17_IRQHandler)(void);
    void (*pI2C1_IRQHandler)(void);
    void (*pReserved4_IRQHandler)(void);
    void (*pSPI1_IRQHandler)(void);
    void (*pSPI2_IRQHandler)(void);
    void (*pUART1_IRQHandler)(void);
    void (*pUART2_IRQHandler)(void);
    void (*pReserved5_IRQHandler)(void);
    void (*pCAN_IRQHandler)(void);
    void (*pReserved6_IRQHandler)(void);
}NVIC_TABLE_t;
#elif (MM32F031_Q)
typedef struct
{
    /*   中断处理函数指针表，在中断服务函数中调用函数指针  Total NUM = 0x2D,45D */
    void (*pInitial_SP);        /*   Top of Stack   */
    void (*pReset_Handler)(void);
    void (*pNMI_Handler)(void);
    void (*pHardFault_Handler)(void);
    void (*pMemManage_Handler)(void);
    void (*pBusFault_Handler)(void);
    void (*pUsageFault_Handler)(void);
    void (*pReserved0_Handler)(void);
    void (*pReserved1_Handler)(void);
    void (*pReserved2_Handler)(void);
    void (*pReserved3_Handler)(void);
    void (*pSVC_Handler)(void);
    void (*pDebugMon_Handler)(void);
    void (*pReserved4_Handler)(void);
    void (*pPendSV_Handler)(void);
    void (*pSysTick_Handler)(void);
    void (*pWWDG_IRQHandler)(void);     /*   External Interrupts    */
    void (*pPVD_IRQHandler)(void);
    void (*pReserved0_IRQHandler)(void);
    void (*pFLASH_IRQHandler)(void);
    void (*pRCC_IRQHandler)(void);
    void (*pEXTI0_1_IRQHandler)(void);
    void (*pEXTI2_3_IRQHandler)(void);
    void (*pEXTI4_15_IRQHandler)(void);
    void (*pReserved1_IRQHandler)(void);
    void (*pDMA1_Channel1_IRQHandler)(void);
    void (*pDMA1_Channel2_3_IRQHandler)(void);
    void (*pDMA1_Channel4_5_IRQHandler)(void);
    void (*pADC1_COMP_IRQHandler)(void);
    void (*pTIM1_BRK_UP_TRG_COM_IRQHandler)(void);
    void (*pTIM1_CC_IRQHandler)(void);
    void (*pTIM2_IRQHandler)(void);
    void (*pTIM3_IRQHandler)(void);
    void (*pReserved2_IRQHandler)(void);
    void (*pReserved3_IRQHandler)(void);
    void (*pTIM14_IRQHandler)(void);
    void (*pReserved4_IRQHandler)(void);
    void (*pTIM16_IRQHandler)(void);
    void (*pTIM17_IRQHandler)(void);
    void (*pI2C1_IRQHandler)(void);
    void (*pReserved5_IRQHandler)(void);
    void (*pSPI1_IRQHandler)(void);
    void (*pSPI2_IRQHandler)(void);
    void (*pUART1_IRQHandler)(void);
    void (*pUART2_IRQHandler)(void);
}NVIC_TABLE_t;
#elif (MM32F003_Q)
typedef struct
{
    /*   中断处理函数指针表，在中断服务函数中调用函数指针  Total NUM = 0x2D,45D */
    void (*pInitial_SP);        /*   Top of Stack   */
    void (*pReset_Handler)(void);
    void (*pNMI_Handler)(void);
    void (*pHardFault_Handler)(void);
    void (*pMemManage_Handler)(void);
    void (*pBusFault_Handler)(void);
    void (*pUsageFault_Handler)(void);
    void (*pReserved0_Handler)(void);
    void (*pReserved1_Handler)(void);
    void (*pReserved2_Handler)(void);
    void (*pReserved3_Handler)(void);
    void (*pSVC_Handler)(void);
    void (*pDebugMon_Handler)(void);
    void (*pReserved4_Handler)(void);
    void (*pPendSV_Handler)(void);
    void (*pSysTick_Handler)(void);
    void (*pWWDG_IRQHandler)(void);     /*   External Interrupts    */
    void (*pPVD_IRQHandler)(void);
    void (*pReserved0_IRQHandler)(void);
    void (*pFLASH_IRQHandler)(void);
    void (*pRCC_IRQHandler)(void);
    void (*pEXTI0_1_IRQHandler)(void);
    void (*pEXTI2_3_IRQHandler)(void);
    void (*pEXTI4_15_IRQHandler)(void);
    void (*pReserved1_IRQHandler)(void);
    void (*pDMA1_Channel1_IRQHandler)(void);
    void (*pDMA1_Channel2_3_IRQHandler)(void);
    void (*pDMA1_Channel4_5_IRQHandler)(void);
    void (*pADC1_IRQHandler)(void);
    void (*pTIM1_BRK_UP_TRG_COM_IRQHandler)(void);
    void (*pTIM1_CC_IRQHandler)(void);
    void (*pTIM2_IRQHandler)(void);
    void (*pTIM3_IRQHandler)(void);
    void (*pReserved2_IRQHandler)(void);
    void (*pReserved3_IRQHandler)(void);
    void (*pTIM14_IRQHandler)(void);
    void (*pReserved4_IRQHandler)(void);
    void (*pTIM16_IRQHandler)(void);
    void (*pTIM17_IRQHandler)(void);
    void (*pI2C1_IRQHandler)(void);
    void (*pReserved5_IRQHandler)(void);
    void (*pReserved6_IRQHandler)(void);
    void (*pSPI2_IRQHandler)(void);
    void (*pReserved7_IRQHandler)(void);
    void (*pUART2_IRQHandler)(void);
}NVIC_TABLE_t;
#elif (MM32SPIN05_Q)
typedef struct
{
    /*   中断处理函数指针表，在中断服务函数中调用函数指针  Total NUM = 0x2D,45D */
    void (*pInitial_SP);        /*   Top of Stack   */
    void (*pReset_Handler)(void);
    void (*pNMI_Handler)(void);
    void (*pHardFault_Handler)(void);
    void (*pMemManage_Handler)(void);
    void (*pBusFault_Handler)(void);
    void (*pUsageFault_Handler)(void);
    void (*pReserved0_Handler)(void);
    void (*pReserved1_Handler)(void);
    void (*pReserved2_Handler)(void);
    void (*pReserved3_Handler)(void);
    void (*pSVC_Handler)(void);
    void (*pDebugMon_Handler)(void);
    void (*pReserved4_Handler)(void);
    void (*pPendSV_Handler)(void);
    void (*pSysTick_Handler)(void);
    void (*pWWDG_IRQHandler)(void);     /*   External Interrupts    */
    void (*pPVD_IRQHandler)(void);
    void (*pReserved0_IRQHandler)(void);
    void (*pFLASH_IRQHandler)(void);
    void (*pRCC_IRQHandler)(void);
    void (*pEXTI0_1_IRQHandler)(void);
    void (*pEXTI2_3_IRQHandler)(void);
    void (*pEXTI4_15_IRQHandler)(void);
    void (*pHWDIV_IRQHandler)(void);
    void (*pDMA1_Channel1_IRQHandler)(void);
    void (*pDMA1_Channel2_3_IRQHandler)(void);
    void (*pDMA1_Channel4_5_IRQHandler)(void);
    void (*pADC1_COMP_IRQHandler)(void);
    void (*pTIM1_BRK_UP_TRG_COM_IRQHandler)(void);
    void (*pTIM1_CC_IRQHandler)(void);
    void (*pTIM2_IRQHandler)(void);
    void (*pTIM3_IRQHandler)(void);
    void (*pReserved2_IRQHandler)(void);
    void (*pReserved3_IRQHandler)(void);
    void (*pTIM14_IRQHandler)(void);
    void (*pReserved4_IRQHandler)(void);
    void (*pTIM16_IRQHandler)(void);
    void (*pTIM17_IRQHandler)(void);
    void (*pI2C1_IRQHandler)(void);
    void (*pReserved5_IRQHandler)(void);
    void (*pSPI1_IRQHandler)(void);
    void (*pSPI2_IRQHandler)(void);
    void (*pUART1_IRQHandler)(void);
    void (*pUART2_IRQHandler)(void);
}NVIC_TABLE_t;
#elif (MM32SPIN27_P)
typedef struct
{
    /*   中断处理函数指针表，在中断服务函数中调用函数指针  Total NUM = 0x2D,45D */
    void (*pInitial_SP);      /*   Top of Stack   */
    void (*pReset_Handler)(void);
    void (*pNMI_Handler)(void);
    void (*pHardFault_Handler)(void);
    void (*pMemManage_Handler)(void);
    void (*pBusFault_Handler)(void);
    void (*pUsageFault_Handler)(void);
    void (*pReserved0_Handler)(void);
    void (*pReserved1_Handler)(void);
    void (*pReserved2_Handler)(void);
    void (*pReserved3_Handler)(void);
    void (*pSVC_Handler)(void);
    void (*pDebugMon_Handler)(void);
    void (*pReserved4_Handler)(void);
    void (*pPendSV_Handler)(void);
    void (*pSysTick_Handler)(void);
    void (*pWWDG_IRQHandler)(void);      /*   External Interrupts    */
    void (*pPVD_IRQHandler)(void);
    void (*pPWM_IRQHandler)(void);
    void (*pFLASH_IRQHandler)(void);
    void (*pRCC_IRQHandler)(void);
    void (*pEXTI0_1_IRQHandler)(void);
    void (*pEXTI2_3_IRQHandler)(void);
    void (*pEXTI4_15_IRQHandler)(void);
    void (*pHWDIV_IRQHandler)(void);
    void (*pDMA1_Channel1_IRQHandler)(void);
    void (*pDMA1_Channel2_3_IRQHandler)(void);
    void (*pDMA1_Channel4_5_IRQHandler)(void);
    void (*pADC1_IRQHandler)(void);
    void (*pTIM1_BRK_UP_TRG_COM_IRQHandler)(void);
    void (*pTIM1_CC_IRQHandler)(void);
    void (*pTIM2_IRQHandler)(void);
    void (*pTIM3_IRQHandler)(void);
    void (*pTIM8_BRK_UP_TRG_COM_IRQHandler)(void);
    void (*pTIM8_CC_IRQHandler)(void);
    void (*pTIM14_IRQHandler)(void);
    void (*pADC2_IRQHandler)(void);
    void (*pTIM16_IRQHandler)(void);
    void (*pTIM17_IRQHandler)(void);
    void (*pI2C1_IRQHandler)(void);
    void (*pCOMP1_2_3_4_5_IRQHandler)(void);
    void (*pSPI1_IRQHandler)(void);
    void (*pSPI2_IRQHandler)(void);
    void (*pUART1_IRQHandler)(void);
    void (*pUART2_IRQHandler)(void);
}NVIC_TABLE_t;
#elif (MM32F031_N)
typedef struct
{
    /*   中断处理函数指针表，在中断服务函数中调用函数指针  Total NUM = 0x2D,45D */
    void (*pInitial_SP);    /*   Top of Stack   */
    void (*pReset_Handler)(void);
    void (*pNMI_Handler)(void);
    void (*pHardFault_Handler)(void);
    void (*pReserved0_Handler)(void);
    void (*pReserved1_Handler)(void);
    void (*pReserved2_Handler)(void);
    void (*pReserved3_Handler)(void);
    void (*pReserved4_Handler)(void);
    void (*pReserved5_Handler)(void);
    void (*pReserved6_Handler)(void);
    void (*pSVC_Handler)(void);
    void (*pReserved7_Handler)(void);
    void (*pReserved8_Handler)(void);
    void (*pPendSV_Handler)(void);
    void (*pSysTick_Handler)(void);
    void (*pWWDG_IRQHandler)(void);     /*   External Interrupts    */
    void (*pPVD_IRQHandler)(void);
    void (*pRTC_IRQHandler)(void);
    void (*pFLASH_IRQHandler)(void);
    void (*pRCC_IRQHandler)(void);
    void (*pEXTI0_1_IRQHandler)(void);
    void (*pEXTI2_3_IRQHandler)(void);
    void (*pEXTI4_15_IRQHandler)(void);
    void (*pReserved0_IRQHandler)(void);
    void (*pDMA1_Channel1_IRQHandler)(void);
    void (*pDMA1_Channel2_3_IRQHandler)(void);
    void (*pDMA1_Channel4_5_IRQHandler)(void);
    void (*pADC1_IRQHandler)(void);
    void (*pTIM1_BRK_UP_TRG_COM_IRQHandler)(void);
    void (*pTIM1_CC_IRQHandler)(void);
    void (*pTIM2_IRQHandler)(void);
    void (*pTIM3_IRQHandler)(void);
    void (*pReserved1_IRQHandler)(void);
    void (*pReserved2_IRQHandler)(void);
    void (*pTIM14_IRQHandler)(void);
    void (*pReserved3_IRQHandler)(void);
    void (*pTIM16_IRQHandler)(void);
    void (*pTIM17_IRQHandler)(void);
    void (*pI2C1_IRQHandler)(void);
    void (*pReserved4_IRQHandler)(void);
    void (*pSPI1_IRQHandler)(void);
    void (*pSPI2_IRQHandler)(void);
    void (*pUART1_IRQHandler)(void);
    void (*pUART2_IRQHandler)(void);
}NVIC_TABLE_t;
#elif (MM32L0XX_N)
typedef struct
{
    /*   中断处理函数指针表，在中断服务函数中调用函数指针  Total NUM = 0x30,48D */
    void (*pInitial_SP);    /*   Top of Stack   */
    void (*pReset_Handler)(void);
    void (*pNMI_Handler)(void);
    void (*pHardFault_Handler)(void);
    void (*pReserved0_Handler)(void);
    void (*pReserved1_Handler)(void);
    void (*pReserved2_Handler)(void);
    void (*pReserved3_Handler)(void);
    void (*pReserved4_Handler)(void);
    void (*pReserved5_Handler)(void);
    void (*pReserved6_Handler)(void);
    void (*pSVC_Handler)(void);
    void (*pReserved7_Handler)(void);
    void (*pReserved8_Handler)(void);
    void (*pPendSV_Handler)(void);
    void (*pSysTick_Handler)(void);
    void (*pWWDG_IRQHandler)(void);     /*   External Interrupts    */
    void (*pPVD_IRQHandler)(void);
    void (*pRTC_IRQHandler)(void);
    void (*pFLASH_IRQHandler)(void);
    void (*pRCC_CRS_IRQHandler)(void);
    void (*pEXTI0_1_IRQHandler)(void);
    void (*pEXTI2_3_IRQHandler)(void);
    void (*pEXTI4_15_IRQHandler)(void);
    void (*pReserved0_IRQHandler)(void);
    void (*pDMA1_Channel1_IRQHandler)(void);
    void (*pDMA1_Channel2_3_IRQHandler)(void);
    void (*pDMA1_Channel4_5_IRQHandler)(void);
    void (*pADC_COMP_IRQHandler)(void);
    void (*pTIM1_BRK_UP_TRG_COM_IRQHandler)(void);
    void (*pTIM1_CC_IRQHandler)(void);
    void (*pTIM2_IRQHandler)(void);
    void (*pTIM3_IRQHandler)(void);
    void (*pReserved1_IRQHandler)(void);
    void (*pReserved2_IRQHandler)(void);
    void (*pTIM14_IRQHandler)(void);
    void (*pReserved3_IRQHandler)(void);
    void (*pTIM16_IRQHandler)(void);
    void (*pTIM17_IRQHandler)(void);
    void (*pI2C1_IRQHandler)(void);
    void (*pReserved4_IRQHandler)(void);
    void (*pSPI1_IRQHandler)(void);
    void (*pSPI2_IRQHandler)(void);
    void (*pUART1_IRQHandler)(void);
    void (*pUART2_IRQHandler)(void);
    void (*pAES_IRQHandler)(void);
    void (*pCAN_IRQHandler)(void);
    void (*pUSB_IRQHandler)(void);
}NVIC_TABLE_t;
#else
/*   中断处理函数指针表，在中断服务函数中调用函数指针   */
#endif

/* Exported variables -------------------------------------------------------*/

EXTERN NVIC_TABLE_t tNVIC_Table ;

/* Exported function prototypes ------------------------------------------------*/

#ifdef __cplusplus
}
#endif


#endif


/******************* (C) COPYRIGHT 2020 ***************************/

