#ifndef BSP_SYSTEM_INIT
#define BSP_SYSTEM_INIT

// #define STM32F10X_CL

#include "stm32f1xx.h"
#include <stm32f1xx_hal.h>

#ifdef STM32F10X_CL
    /**
     * @brief user set value like 107, in laste systemclocck value is
     * HSE / PREDIV2_VALUE * PLL2MUL_VALUE / PREDIV1_VALUE * PLLMUL_VALUE
     * 
     */
    #define PREDIV2_VALUE 5
    #define PLL2MUL_VALUE 8
    #define PREDIV1_VALUE 5
    #define PLLMUL_VALUE  9

    #define PREDIV2_CFG (RCC_CFGR2_PREDIV2_DIV ## 5)
    #define PLL2MUL_CFG (RCC_CFGR2_PLL2MUL ## 8)
    #define PREDIV1_CFG (RCC_CFGR2_PREDIV1_DIV ## 5)
    #define PLLMUL_CFG  (RCC_CFGR_PLLMULL ## 9)
#else
    /**
     * @brief user set value like 103, in laste systemclocck value is
     * HSE / PREDIV2_VALUE * PLL2MUL_VALUE / PREDIV1_VALUE * PLLMUL_VALUE
     * 
     */
    #define PREDIV1_VALUE 1
    #define PLLMUL_VALUE  9

    #define PREDIV1_CFG (RCC_CFGR_PPRE1_DIV##1)
    #define PLLMUL_CFG  (RCC_CFGR_PLLMULL#9)
#endif

/**
 * @brief auto calculate NOT modify
 */

// #define TE text
// #define PREDIV2_TEMP PREDIV2_VALUE
// #define STR(AD) #AD

// #define TO_STRING(x)  (#x)
// #define TO_VALUE(x)  (#x)
// #define LINK_STR(x,y) (x##y)
// #define PREDIV2_CFG (RCC_CFGR2_PREDIV2_DIV##5)
// #define PLL2MUL_CFG (##RCC_CFGR2_PLL2MUL#PLL2MUL_VALUE)
// #define PREDIV1_CFG (##RCC_CFGR_PPRE1_DIV#PREDIV1_VALUE)
// #define PLLMUL_CFG  (##RCC_CFGR_PLLMULL#PLLMUL_VALUE)




void setSystemClock(void);
void enable_GPIOx_clock(GPIO_TypeDef *GPIOx);

#endif