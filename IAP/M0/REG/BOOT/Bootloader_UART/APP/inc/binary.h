/******************************************************************************
 * @file    binary.h
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


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _BINARY_H_
#define _BINARY_H_


#ifdef __cplusplus
extern "C" {
#endif


#undef  EXTERN


#ifdef  __BINARY_C__
#define EXTERN
#else
#define EXTERN extern
#endif

/* Includes ------------------------------------------------------------------*/
#include "config.h"
#include "stdbool.h"

/* Exported variables -------------------------------------------------------*/
/* Exported function prototypes ---------------------------------------------*/
/* Exported macro ---------------------------------------------*/
/* Exported constants -------------------------------------------------------*/

#define SET     true        
#define RESET   false       
#define AUTO    true        
#define MANUAL  false       
    
/* Exported typedef -------------------------------------------------------*/

//! \name finit state machine state
//! @{
typedef enum {
   fsm_rt_err          = -1,    //!< fsm error
   fsm_rt_cpl          = 0,     //!< fsm complete
   fsm_rt_on_going     = 1,     //!< fsm on-going
} fsm_rt_t;
//! @}

typedef struct {
    bool bAutoReset;    //  reset type
    bool bIsSet;        //  initial state 
} event_t;


/* Exported function prototypes ---------------------------------------------*/

EXTERN int32_t Binary_Receive (void);


#ifdef  __cplusplus
}
#endif

#endif 

/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

