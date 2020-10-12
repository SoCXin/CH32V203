////////////////////////////////////////////////////////////////////////////////
/// @file     SYSTEM_MM32.C
/// @author   AE TEAM
/// @brief    THIS FILE PROVIDES ALL THE SYSTEM FUNCTIONS.
////////////////////////////////////////////////////////////////////////////////
/// @attention
///
/// THE EXISTING FIRMWARE IS ONLY FOR REFERENCE, WHICH IS DESIGNED TO PROVIDE
/// CUSTOMERS WITH CODING INFORMATION ABOUT THEIR PRODUCTS SO THEY CAN SAVE
/// TIME. THEREFORE, MINDMOTION SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT OR
/// CONSEQUENTIAL DAMAGES ABOUT ANY CLAIMS ARISING OUT OF THE CONTENT OF SUCH
/// HARDWARE AND/OR THE USE OF THE CODING INFORMATION CONTAINED HEREIN IN
/// CONNECTION WITH PRODUCTS MADE BY CUSTOMERS.
///
/// <H2><CENTER>&COPY; COPYRIGHT MINDMOTION </CENTER></H2>
////////////////////////////////////////////////////////////////////////////////

// Define to prevent recursive inclusion  --------------------------------------
#define _SYSTEM_MM32_C_

// Files includes  -------------------------------------------------------------

/// @addtogroup CMSIS
/// @{

#include "mm32_device.h"


/// @}



/// @}


/// Uncomment the line corresponding to the desired System clock (SYSCLK)
/// frequency (after reset the HSI is used as SYSCLK source)
///
/// IMPORTANT NOTE:
/// ==============
/// 1. After each device reset the HSI is used as System clock source.
///
/// 2. Please make sure that the selected System clock doesn't exceed your device's
/// maximum frequency.
///
/// 3. If none of the define below is enabled, the HSI is used as System clock
/// source.
///
/// 4. The System clock configuration functions provided within this file assume that:
/// - For Low, Medium and High density Value line devices an external 8MHz
/// crystal is used to drive the System clock.
/// - For Low, Medium and High density devices an external 8MHz crystal is
/// used to drive the System clock.
/// - For Connectivity line devices an external 25MHz crystal is used to drive
/// the System clock.
/// If you are using different crystal you have to adapt those functions accordingly.


//#define SYSCLK_FREQ_HSE    HSE_VALUE
//#define SYSCLK_FREQ_24MHz  24000000
//#define SYSCLK_FREQ_36MHz  36000000
//#define SYSCLK_FREQ_48MHz  48000000
//#define SYSCLK_FREQ_72MHz  72000000

//#define SYSCLK_HSI_24MHz  24000000
//#define SYSCLK_HSI_36MHz  36000000
#define SYSCLK_HSI_48MHz  48000000
//#define SYSCLK_HSI_72MHz  72000000

/// Uncomment the following line if you need to relocate your vector Table in
/// Internal SRAM.
///#define VECT_TAB_SRAM
#define VECT_TAB_OFFSET  0x0
/// Vector Table base offset field.
/// This value must be a multiple of 0x200.


/// @}


///////////////////////////////////////////////////////////////
///Clock Definitions
///////////////////////////////////////////////////////////////
#if defined SYSCLK_FREQ_HSE
uint32_t SystemCoreClock         = SYSCLK_FREQ_HSE;
#elif defined SYSCLK_FREQ_24MHz
uint32_t SystemCoreClock         = SYSCLK_FREQ_24MHz;
#elif defined SYSCLK_FREQ_36MHz
uint32_t SystemCoreClock         = SYSCLK_FREQ_36MHz;
#elif defined SYSCLK_FREQ_48MHz
uint32_t SystemCoreClock         = SYSCLK_FREQ_48MHz;
#elif defined SYSCLK_FREQ_72MHz
uint32_t SystemCoreClock         = SYSCLK_FREQ_72MHz;


#elif defined SYSCLK_HSI_24MHz
uint32_t SystemCoreClock         = SYSCLK_HSI_24MHz;
#elif defined SYSCLK_HSI_36MHz
uint32_t SystemCoreClock         = SYSCLK_HSI_36MHz;
#elif defined SYSCLK_HSI_48MHz
uint32_t SystemCoreClock         = SYSCLK_HSI_48MHz;
#elif defined SYSCLK_HSI_72MHz
uint32_t SystemCoreClock         = SYSCLK_HSI_72MHz;
#else //HSI Selected as System Clock source 
uint32_t SystemCoreClock         = HSI_VALUE;
#endif

__I uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};

/// @}


static void SetSysClock(void);

#if defined SYSCLK_FREQ_HSE
static void SetSysClockToHSE(void);
#elif defined SYSCLK_FREQ_24MHz
static void SetSysClockTo24(void);
#elif defined SYSCLK_FREQ_36MHz
static void SetSysClockTo36(void);
#elif defined SYSCLK_FREQ_48MHz
static void SetSysClockTo48(void);
#elif defined SYSCLK_FREQ_72MHz
static void SetSysClockTo72(void);

#elif defined SYSCLK_HSI_24MHz
static void SetSysClockTo24_HSI(void);
#elif defined SYSCLK_HSI_36MHz
static void SetSysClockTo36_HSI(void);
#elif defined SYSCLK_HSI_48MHz
static void SetSysClockTo48_HSI(void);
#elif defined SYSCLK_HSI_72MHz
static void SetSysClockTo72_HSI(void);
#endif

#ifdef DATA_IN_ExtSRAM
static void SystemInit_ExtMemCtl(void);
#endif //DATA_IN_ExtSRAM 


/// @}



/// @brief  Setup the microcontroller system
///         Initialize the Embedded Flash Interface, the PLL and update the
///         SystemCoreClock variable.
/// @note   This function should be used only after reset.
/// @param  None
/// @retval None

void SystemInit (void)
{
    //Reset the RCC clock configuration to the default reset state(for debug purpose)
    //Set HSION bit
    RCC->CR |= (uint32_t)0x00000001;

    //Reset SW, HPRE, PPRE1, PPRE2, ADCPRE and MCO bits
    RCC->CFGR &= (uint32_t)0xF8FFC00C;

    //Reset HSEON, CSSON and PLLON bits
    RCC->CR &= (uint32_t)0xFEF6FFFF;

    //Reset HSEBYP bit
    RCC->CR &= (uint32_t)0xFFFBFFFF;

    //Reset PLLSRC, PLLXTPRE, PLLMUL and USBPRE/OTGFSPRE bits
    RCC->CFGR &= (uint32_t)0xFF3CFFFF;
    RCC->CR &= (uint32_t)0x008FFFFF;

    //Disable all interrupts and clear pending bits
    RCC->CIR = 0x009F0000;

    //Configure the System clock frequency, HCLK, PCLK2 and PCLK1 prescalers
    //Configure the Flash Latency cycles and enable prefetch buffer
    SetSysClock();
}


/// @brief  use to return the pllm&plln.
/// @param  pllclkSourceFrq : PLL source clock frquency;
///         pllclkFrq : Target PLL clock frquency;
///         plln : PLL factor PLLN
///         pllm : PLL factor PLLM
/// @retval amount of error
u32 AutoCalPllFactor(u32 pllclkSourceFrq, u32 pllclkFrq, u8* plln, u8* pllm)
{
    u32 n, m;
    u32 tempFrq;
    u32 minDiff = pllclkFrq;
    u8  flag = 0;
    for(m = 0; m < 4 ; m++) {
        for(n = 0; n < 64 ; n++) {
            tempFrq =  pllclkSourceFrq * (n + 1) / (m + 1);
            tempFrq = (tempFrq >  pllclkFrq) ? (tempFrq - pllclkFrq) : (pllclkFrq - tempFrq) ;

            if(minDiff > tempFrq) {
                minDiff =  tempFrq;
                *plln = n;
                *pllm = m;
            }
            if(minDiff == 0) {
                flag = 1;
                break;
            }
        }
        if(flag != 0) {
            break;
        }
    }
    return  minDiff;
}

/// @brief  Configures the System clock frequency, HCLK, PCLK2 and PCLK1 prescalers.
/// @param  None
/// @retval None

static void SetSysClock(void)
{
#ifdef SYSCLK_FREQ_HSE
    SetSysClockToHSE();
#elif defined SYSCLK_FREQ_24MHz
    SetSysClockTo24();
#elif defined SYSCLK_FREQ_36MHz
    SetSysClockTo36();
#elif defined SYSCLK_FREQ_48MHz
    SetSysClockTo48();
#elif defined SYSCLK_FREQ_72MHz
    SetSysClockTo72();

#elif defined SYSCLK_HSI_24MHz
    SetSysClockTo24_HSI();
#elif defined SYSCLK_HSI_36MHz
    SetSysClockTo36_HSI();
#elif defined SYSCLK_HSI_48MHz
    SetSysClockTo48_HSI();
#elif defined SYSCLK_HSI_72MHz
    SetSysClockTo72_HSI();
#endif

    //If none of the define above is enabled, the HSI is used as System clock
    //source (default after reset)
}

#ifdef SYSCLK_FREQ_HSE

/// @brief  Selects HSE as System clock source and configure HCLK, PCLK2
///         and PCLK1 prescalers.
/// @note   This function should be used only after reset.
/// @param  None
/// @retval None

static void SetSysClockToHSE(void)
{
    __IO uint32_t StartUpCounter = 0, HSEStatus = 0;
    int i;

    //SYSCLK, HCLK, PCLK2 and PCLK1 configuration ---------------------------
    //Enable HSE
    RCC->CR |= ((uint32_t)RCC_CR_HSEON);

    //Wait till HSE is ready and if Time out is reached exit
    do {
        HSEStatus = RCC->CR & RCC_CR_HSERDY;
        StartUpCounter++;
    } while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

    if ((RCC->CR & RCC_CR_HSERDY) != RESET) {
        HSEStatus = (uint32_t)0x01;
        i = 2000; while(i--);
    }
    else {
        HSEStatus = (uint32_t)0x00;
    }

    if (HSEStatus == (uint32_t)0x01) {
        //Enable Prefetch Buffer
        FLASH->ACR |= FLASH_ACR_PRFTBE;

        //Flash 0 wait state ,bit0~2
        FLASH->ACR &= ~FLASH_ACR_LATENCY;
        //HCLK = SYSCLK
        RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;

        //PCLK2 = HCLK
        RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;

        //PCLK1 = HCLK
        RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV1;

        //Select HSE as system clock source
        RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
        RCC->CFGR |= (uint32_t)RCC_CFGR_SW_HSE;

        //Wait till HSE is used as system clock source
        while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x04) {
        }
    }
    else {
        //If HSE fails to start-up, the application will have wrong clock
        //configuration. User can add here some code to deal with this error
    }
}
#elif defined SYSCLK_FREQ_24MHz

/// @brief  Sets System clock frequency to 24MHz and configure HCLK, PCLK2
///         and PCLK1 prescalers.
/// @note   This function should be used only after reset.
/// @param  None
/// @retval None

static void SetSysClockTo24(void)
{
    __IO uint32_t StartUpCounter = 0, HSEStatus = 0;
    int i;

    //SYSCLK, HCLK, PCLK2 and PCLK1 configuration ---------------------------
    //Enable HSE
    RCC->CR |= ((uint32_t)RCC_CR_HSEON);

    //Wait till HSE is ready and if Time out is reached exit
    do {
        HSEStatus = RCC->CR & RCC_CR_HSERDY;
        StartUpCounter++;
    } while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

    if ((RCC->CR & RCC_CR_HSERDY) != RESET) {
        HSEStatus = (uint32_t)0x01;
        i = 2000; while(i--);
    }
    else {
        HSEStatus = (uint32_t)0x00;
    }

    if (HSEStatus == (uint32_t)0x01) {
        //Enable Prefetch Buffer
        FLASH->ACR |= FLASH_ACR_PRFTBE;
        //Flash 0 wait state ,bit0~2
        FLASH->ACR &= ~FLASH_ACR_LATENCY;
        FLASH->ACR |= FLASH_ACR_LATENCY_1;
        //HCLK = SYSCLK
        RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;

        //PCLK2 = HCLK
        RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;

        //PCLK1 = HCLK
        RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV1;

        // PLL configuration:  = (HSE ) * (2+1) = 24 MHz
        RCC->CFGR &= ~((uint32_t)(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE));
        RCC->CR &= ~((uint32_t)(RCC_CR_PLLDIV | RCC_CR_PLLON | RCC_CR_PLLRDY | RCC_CR_PLLMUL));

        RCC->CFGR |= (uint32_t)RCC_CFGR_PLLSRC;
        RCC->CR   |= (0x02 << RCC_CR_PLLMUL_Pos);

        //Enable PLL
        RCC->CR |= RCC_CR_PLLON;

        //Wait till PLL is ready
        while((RCC->CR & RCC_CR_PLLRDY) == 0) {
        }

        //Select PLL as system clock source
        RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
        RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;

        //Wait till PLL is used as system clock source
        while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x08) {
        }
    }
    else {
        //If HSE fails to start-up, the application will have wrong clock
        //configuration. User can add here some code to deal with this error
    }
}
#elif defined SYSCLK_FREQ_36MHz

/// @brief  Sets System clock frequency to 36MHz and configure HCLK, PCLK2
///         and PCLK1 prescalers.
/// @note   This function should be used only after reset.
/// @param  None
/// @retval None

static void SetSysClockTo36(void)
{
    int i;
    __IO uint32_t StartUpCounter = 0, HSEStatus = 0;

    //SYSCLK, HCLK, PCLK2 and PCLK1 configuration ---------------------------
    //Enable HSE
    RCC->CR |= ((uint32_t)RCC_CR_HSEON);

    //Wait till HSE is ready and if Time out is reached exit
    do {
        HSEStatus = RCC->CR & RCC_CR_HSERDY;
        StartUpCounter++;
    } while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

    if ((RCC->CR & RCC_CR_HSERDY) != RESET) {
        HSEStatus = (uint32_t)0x01;
        i = 2000; while(i--);
    }
    else {
        HSEStatus = (uint32_t)0x00;
    }

    if (HSEStatus == (uint32_t)0x01) {
        //Enable Prefetch Buffer
        FLASH->ACR |= FLASH_ACR_PRFTBE;

        //Flash 0 wait state ,bit0~2
        FLASH->ACR &= ~FLASH_ACR_LATENCY;
        FLASH->ACR |= FLASH_ACR_LATENCY_1;
        //HCLK = SYSCLK
        RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;

        //PCLK2 = HCLK
        RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;

        //PCLK1 = HCLK
        RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV1;

        // PLL configuration:  = (HSE ) * (8+1)/(1+1) = 36 MHz
        RCC->CFGR &= ~((uint32_t)(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE));
        RCC->CR &= ~((uint32_t)(RCC_CR_PLLDIV | RCC_CR_PLLON | RCC_CR_PLLRDY | RCC_CR_PLLMUL));

        RCC->CFGR |= (uint32_t)RCC_CFGR_PLLSRC ;
        RCC->CR   |= ((0x08 << RCC_CR_PLLMUL_Pos) | (0x01 << RCC_CR_PLLDIV_Pos));

        //Enable PLL
        RCC->CR |= RCC_CR_PLLON;

        //Wait till PLL is ready
        while((RCC->CR & RCC_CR_PLLRDY) == 0) {
        }

        //Select PLL as system clock source
        RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
        RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;

        //Wait till PLL is used as system clock source
        while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x08) {
        }
    }
    else {
        //If HSE fails to start-up, the application will have wrong clock
        //configuration. User can add here some code to deal with this error
    }
}
#elif defined SYSCLK_FREQ_48MHz

/// @brief  Sets System clock frequency to 48MHz and configure HCLK, PCLK2
///         and PCLK1 prescalers.
/// @note   This function should be used only after reset.
/// @param  None
/// @retval None

static void SetSysClockTo48(void)
{
    __IO uint32_t StartUpCounter = 0, HSEStatus = 0;
    int i;
    uint8_t plln, pllm;
    //SYSCLK, HCLK, PCLK2 and PCLK1 configuration ---------------------------
    //Enable HSE
    RCC->CR |= ((uint32_t)RCC_CR_HSEON);

    //Wait till HSE is ready and if Time out is reached exit
    do {
        HSEStatus = RCC->CR & RCC_CR_HSERDY;
        StartUpCounter++;
    } while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

    if ((RCC->CR & RCC_CR_HSERDY) != RESET) {
        HSEStatus = (uint32_t)0x01;
        i = 2000; while(i--);
    }
    else {
        HSEStatus = (uint32_t)0x00;
    }

    if (HSEStatus == (uint32_t)0x01) {
        //Enable Prefetch Buffer
        FLASH->ACR |= FLASH_ACR_PRFTBE;
        //Flash 0 wait state ,bit0~2
        FLASH->ACR &= ~FLASH_ACR_LATENCY;
        FLASH->ACR |= FLASH_ACR_LATENCY_2;
        //HCLK = SYSCLK
        RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;

        //PCLK2 = HCLK
        RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;

        //PCLK1 = HCLK
        RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV2;

        // PLL configuration:  = (HSE ) * (5+1) = 48MHz
        RCC->CFGR &= ~((uint32_t)(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE));
        RCC->CR &= ~((uint32_t)(RCC_CR_PLLDIV | RCC_CR_PLLON | RCC_CR_PLLRDY | RCC_CR_PLLMUL));

        RCC->CFGR |= (uint32_t ) RCC_CFGR_PLLSRC;
        RCC->CR   |= (0x05 << RCC_CR_PLLMUL_Pos);

        //Enable PLL
        RCC->CR |= RCC_CR_PLLON;

        //Wait till PLL is ready
        while((RCC->CR & RCC_CR_PLLRDY) == 0) {
        }

        //Select PLL as system clock source
        RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
        RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;

        //Wait till PLL is used as system clock source
        while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x08) {
        }
    }
    else {
        //If HSE fails to start-up, the application will have wrong clock
        //configuration. User can add here some code to deal with this error
    }
}
#elif defined SYSCLK_FREQ_72MHz

/// @brief  Sets System clock frequency to XXMHz and configure HCLK, PCLK2
///         and PCLK1 prescalers.
/// @note   This function should be used only after reset.
/// @param  None
/// @retval None

static void SetSysClockTo72(void)
{
    __IO uint32_t StartUpCounter = 0, HSEStatus = 0;
    int i;
    uint32_t temp;
    uint8_t plln, pllm;
    //SYSCLK, HCLK, PCLK2 and PCLK1 configuration ---------------------------
    //Enable HSE
    RCC->CR |= ((uint32_t)RCC_CR_HSEON);

    //Wait till HSE is ready and if Time out is reached exit
    do {
        HSEStatus = RCC->CR & RCC_CR_HSERDY;
        StartUpCounter++;
    } while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

    if ((RCC->CR & RCC_CR_HSERDY) != RESET) {
        HSEStatus = (uint32_t)0x01;
        i = 2000; while(i--);
    }
    else {
        HSEStatus = (uint32_t)0x00;
    }

    if (HSEStatus == (uint32_t)0x01) {
        SystemCoreClock         = SYSCLK_FREQ_72MHz;
        //Enable Prefetch Buffer
        FLASH->ACR |= FLASH_ACR_PRFTBE;
        //Flash 0 wait state ,bit0~2
        FLASH->ACR &= ~FLASH_ACR_LATENCY;
        temp = (SystemCoreClock - 1) / 24000000;
        FLASH->ACR |= temp;

        RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;
        //HCLK = SYSCLK

        //PCLK2 = HCLK
        RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;

        if(SystemCoreClock > 36000000) {
            //PCLK1 = HCLK/2
            RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV2;
        }
        else {
            //PCLK1 = HCLK
            RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV2;
        }

        RCC->CFGR &= ~((uint32_t)(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE));
        RCC->CR &= ~((uint32_t)(RCC_CR_PLLDIV | RCC_CR_PLLON | RCC_CR_PLLRDY | RCC_CR_PLLMUL));

        RCC->CFGR |= (uint32_t ) RCC_CFGR_PLLSRC ;

        AutoCalPllFactor(8000000, SystemCoreClock, &plln, &pllm);
        RCC->CR &= ~((uint32_t)(RCC_CR_PLLDIV | RCC_CR_PLLON | RCC_CR_PLLRDY | RCC_CR_PLLMUL));
        RCC->CR |= (plln << RCC_CR_PLLMUL_Pos) | (pllm << RCC_CR_PLLDIV_Pos);

        //Enable PLL
        RCC->CR |= RCC_CR_PLLON;

        //Wait till PLL is ready
        while((RCC->CR & RCC_CR_PLLRDY) == 0) {
        }

        //Select PLL as system clock source
        RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
        RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;

        //Wait till PLL is used as system clock source
        while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x08) {
        }
    }
    else {
        //If HSE fails to start-up, the application will have wrong clock
        //configuration. User can add here some code to deal with this error
    }
}
#elif defined SYSCLK_HSI_24MHz
void SetSysClockTo24_HSI(void)
{
    __IO uint32_t StartUpCounter = 0, HSIStatus = 0;
    int i;
    unsigned char temp = 0;
    uint8_t plln, pllm;

    //Enable HSI
    RCC->CR |= RCC_CR_HSION;

    //Wait till HSI is ready and if Time out is reached exit
    do {
        HSIStatus = RCC->CR & RCC_CR_HSIRDY;
        StartUpCounter++;
    } while((HSIStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

    if ((RCC->CR & RCC_CR_HSIRDY) != RESET) {
        HSIStatus = (uint32_t)0x01;
        i = 2000; while(i--);
    }
    else {
        HSIStatus = (uint32_t)0x00;
    }

    if (HSIStatus == (uint32_t)0x01) {
        SystemCoreClock         = SYSCLK_HSI_24MHz;
        //Enable Prefetch Buffer
        FLASH->ACR |= FLASH_ACR_PRFTBE;
        //Flash 0 wait state ,bit0~2
        FLASH->ACR &= ~FLASH_ACR_LATENCY;

        temp = (SystemCoreClock - 1) / 24000000;

        FLASH->ACR |= temp;

        //HCLK = SYSCLK
        RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;

        //PCLK2 = HCLK
        RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;

        if(SystemCoreClock > 36000000) {
            //PCLK1 = HCLK/2
            RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV2;
        }
        else {
            //PCLK1 = HCLK
            RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV2;
        }

        RCC->CFGR &= ~((uint32_t)(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE));
        RCC->CR &= ~((uint32_t)(RCC_CR_PLLDIV | RCC_CR_PLLON | RCC_CR_PLLRDY | RCC_CR_PLLMUL));

        AutoCalPllFactor(12000000, SystemCoreClock, &plln, &pllm);
        RCC->CR &= ~((uint32_t)(RCC_CR_PLLDIV | RCC_CR_PLLON | RCC_CR_PLLRDY | RCC_CR_PLLMUL));
        RCC->CR |= (plln << RCC_CR_PLLMUL_Pos) | (pllm << RCC_CR_PLLDIV_Pos);

        //Enable PLL
        RCC->CR |= RCC_CR_PLLON;

        //Wait till PLL is ready
        while(!(RCC->CR & RCC_CR_PLLRDY)){
        }

        //Select PLL as system clock source
        RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
        RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;

        //Wait till PLL is used as system clock source
        while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x08) {
        }
    }
    else{
        //If HSI fails to start-up, the application will have wrong clock
        //configuration. User can add here some code to deal with this error
    }
}

#elif defined SYSCLK_HSI_36MHz
void SetSysClockTo36_HSI(void)
{
    __IO uint32_t StartUpCounter = 0, HSIStatus = 0;
    int i;
    unsigned char temp = 0;
    uint8_t plln, pllm;

    //Enable HSI
    RCC->CR |= RCC_CR_HSION;

    //Wait till HSI is ready and if Time out is reached exit
    do {
        HSIStatus = RCC->CR & RCC_CR_HSIRDY;
        StartUpCounter++;
    } while((HSIStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

    if ((RCC->CR & RCC_CR_HSIRDY) != RESET) {
        HSIStatus = (uint32_t)0x01;
        i = 2000; while(i--);
    }
    else {
        HSIStatus = (uint32_t)0x00;
    }

    if (HSIStatus == (uint32_t)0x01) {
        SystemCoreClock         = SYSCLK_HSI_36MHz;
        //Enable Prefetch Buffer
        FLASH->ACR |= FLASH_ACR_PRFTBE;
        //Flash 0 wait state ,bit0~2
        FLASH->ACR &= ~FLASH_ACR_LATENCY;

        temp = (SystemCoreClock - 1) / 24000000;

        FLASH->ACR |= temp;

        //HCLK = SYSCLK
        RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;

        //PCLK2 = HCLK
        RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;

        if(SystemCoreClock > 36000000) {
            //PCLK1 = HCLK/2
            RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV2;
        }
        else {
            //PCLK1 = HCLK
            RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV2;
        }

        RCC->CFGR &= ~((uint32_t)(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE));
        RCC->CR &= ~((uint32_t)(RCC_CR_PLLDIV | RCC_CR_PLLON | RCC_CR_PLLRDY | RCC_CR_PLLMUL));

        AutoCalPllFactor(12000000, SystemCoreClock, &plln, &pllm);
        RCC->CR &= ~((uint32_t)(RCC_CR_PLLDIV | RCC_CR_PLLON | RCC_CR_PLLRDY | RCC_CR_PLLMUL));
        RCC->CR |= (plln << RCC_CR_PLLMUL_Pos) | (pllm << RCC_CR_PLLDIV_Pos);

        //Enable PLL
        RCC->CR |= RCC_CR_PLLON;

        //Wait till PLL is ready
        while(!(RCC->CR & RCC_CR_PLLRDY)){
        }

        //Select PLL as system clock source
        RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
        RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;

        //Wait till PLL is used as system clock source
        while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x08) {
        }
    }
    else{
        //If HSI fails to start-up, the application will have wrong clock
        //configuration. User can add here some code to deal with this error
    }
}

#elif defined SYSCLK_HSI_48MHz
void SetSysClockTo48_HSI(void)
{
    __IO uint32_t StartUpCounter = 0, HSIStatus = 0;
    int i;
    unsigned char temp = 0;
    uint8_t plln, pllm;

    //Enable HSI
    RCC->CR |= RCC_CR_HSION;

    //Wait till HSI is ready and if Time out is reached exit
    do {
        HSIStatus = RCC->CR & RCC_CR_HSIRDY;
        StartUpCounter++;
    } while((HSIStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

    if ((RCC->CR & RCC_CR_HSIRDY) != RESET) {
        HSIStatus = (uint32_t)0x01;
        i = 2000; while(i--);
    }
    else {
        HSIStatus = (uint32_t)0x00;
    }

    if (HSIStatus == (uint32_t)0x01) {
        SystemCoreClock         = SYSCLK_HSI_48MHz;
        //Enable Prefetch Buffer
        FLASH->ACR |= FLASH_ACR_PRFTBE;
        //Flash 0 wait state ,bit0~2
        FLASH->ACR &= ~FLASH_ACR_LATENCY;

        temp = (SystemCoreClock - 1) / 24000000;

        FLASH->ACR |= temp;

        //HCLK = SYSCLK
        RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;

        //PCLK2 = HCLK
        RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;

        if(SystemCoreClock > 36000000) {
            //PCLK1 = HCLK/2
            RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV2;
        }
        else {
            //PCLK1 = HCLK
            RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV2;
        }

        RCC->CFGR &= ~((uint32_t)(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE));
        RCC->CR &= ~((uint32_t)(RCC_CR_PLLDIV | RCC_CR_PLLON | RCC_CR_PLLRDY | RCC_CR_PLLMUL));

        AutoCalPllFactor(12000000, SystemCoreClock, &plln, &pllm);
        RCC->CR &= ~((uint32_t)(RCC_CR_PLLDIV | RCC_CR_PLLON | RCC_CR_PLLRDY | RCC_CR_PLLMUL));
        RCC->CR |= (plln << RCC_CR_PLLMUL_Pos) | (pllm << RCC_CR_PLLDIV_Pos);

        //Enable PLL
        RCC->CR |= RCC_CR_PLLON;

        //Wait till PLL is ready
        while(!(RCC->CR & RCC_CR_PLLRDY)){
        }

        //Select PLL as system clock source
        RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
        RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;

        //Wait till PLL is used as system clock source
        while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x08) {
        }
    }
    else{
        //If HSI fails to start-up, the application will have wrong clock
        //configuration. User can add here some code to deal with this error
    }
}
#elif defined SYSCLK_HSI_72MHz
void SetSysClockTo72_HSI(void)
{
    __IO uint32_t StartUpCounter = 0, HSIStatus = 0;
    int i;
    uint32_t temp;
    uint8_t plln, pllm;

    //Enable HSI
    RCC->CR |= RCC_CR_HSION;

    //Wait till HSI is ready and if Time out is reached exit
    do {
        HSIStatus = RCC->CR & RCC_CR_HSIRDY;
        StartUpCounter++;
    } while((HSIStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

    if ((RCC->CR & RCC_CR_HSIRDY) != RESET) {
        HSIStatus = (uint32_t)0x01;
        i = 2000; while(i--);
    }
    else {
        HSIStatus = (uint32_t)0x00;
    }

    if (HSIStatus == (uint32_t)0x01) {
        SystemCoreClock         = SYSCLK_HSI_72MHz;
        //Enable Prefetch Buffer
        FLASH->ACR |= FLASH_ACR_PRFTBE;
        //Flash 0 wait state ,bit0~2
        FLASH->ACR &= ~FLASH_ACR_LATENCY;

        temp = (SystemCoreClock - 1) / 24000000;

        FLASH->ACR |= temp;

        //HCLK = SYSCLK
        RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;

        //PCLK2 = HCLK
        RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;

        if(SystemCoreClock > 36000000) {
            //PCLK1 = HCLK/2
            RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV2;
        }
        else {
            //PCLK1 = HCLK
            RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV2;
        }

        RCC->CFGR &= ~((uint32_t)(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE));
        RCC->CR &= ~((uint32_t)(RCC_CR_PLLDIV | RCC_CR_PLLON | RCC_CR_PLLRDY | RCC_CR_PLLMUL));

        AutoCalPllFactor(12000000, SystemCoreClock, &plln, &pllm);
        RCC->CR &= ~((uint32_t)(RCC_CR_PLLDIV | RCC_CR_PLLON | RCC_CR_PLLRDY | RCC_CR_PLLMUL));
        RCC->CR |= (plln << RCC_CR_PLLMUL_Pos) | (pllm << RCC_CR_PLLDIV_Pos);

        //Enable PLL
        RCC->CR |= RCC_CR_PLLON;

        //Wait till PLL is ready
        while((RCC->CR & RCC_CR_PLLRDY) == 0) {
        }

        //Select PLL as system clock source
        RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
        RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;

        //Wait till PLL is used as system clock source
        while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x08) {
        }
    }
    else{
        //If HSI fails to start-up, the application will have wrong clock
        //configuration. User can add here some code to deal with this error
    }
}

#endif


/// @}



/// @}



/// @}



