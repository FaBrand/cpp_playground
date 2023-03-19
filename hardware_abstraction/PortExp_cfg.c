/**
*********************************************************************************
* @file   PortExp_cfg.c
* @brief  Port Expander PCF8575 interface configuration
*
* @details
*
* This file instantiates the product side data and function interfaces required
* for the port expander driver to operate.
*
*********************************************************************************/

/********************************************************************************/
/********************************************************************************/
/* Header files */
#include "PortExp_cfg.h"

#include <stdio.h>
#include <stdlib.h>

#include "PortExp.h"
#include "PortExp_intg.h"
#include "drivers/stm32f3xx_hal.h"

/* IIC bus imported form HAL */
struct I2C_HandleTypeDef hi2c1;

uint8_t HAL_OK = 0x00U;

/* Working variables */
static const stPORTEXP_DevDesc_t stPortExpDesc[PORTEXP_NUM] = {
    {
        PORTEXP_ID__U20,
        {0x40, 0x41},
        0xFFFF,
    },
    {
        PORTEXP_ID__U22,
        {0x44, 0x45},
        0x0000,
    },
    {
        PORTEXP_ID__U21,
        {0x42, 0x43},
        0x0000,
    },
};

/********************************************************************************/
/********************************************************************************/
/* Local Function Prototypes */

/********************************************************************************/
/********************************************************************************/
/* Public Functions */

/** */
uint32_t PORTEXP_GetPortExpCount(void) { return (PORTEXP_NUM); }

/** */
const stPORTEXP_DevDesc_t *PORTEXP_GetPortExpDesc(const uint32_t u32Idx) {
    const stPORTEXP_DevDesc_t *pstDesc = NULL;

    if (u32Idx < PORTEXP_NUM) {
        pstDesc = &stPortExpDesc[u32Idx];
    }

    return pstDesc;
}

/** */
void *PORTEXP_GetMemory(const uint32_t u32Size) {
    return (calloc(u32Size, sizeof(uint8_t)));
}

/** */
bool PORTEXP_Transmit(const uint32_t u32PortExpId, uint8_t *const pu8Data,
                      const uint16_t u16Size) {
    return (HAL_OK == HAL_I2C_Master_Transmit(
                          &hi2c1, stPortExpDesc[u32PortExpId].stAddr.u16WrAddr,
                          pu8Data, u16Size, 100));
}

/** */
bool PORTEXP_Receive(const uint32_t u32PortExpId, uint8_t *const pu8Data,
                     const uint16_t u16Size) {
    return (HAL_OK == HAL_I2C_Master_Receive(
                          &hi2c1, stPortExpDesc[u32PortExpId].stAddr.u16RrAddr,
                          pu8Data, u16Size, 100));
}

/********************************************************************************/
/********************************************************************************/
/* Local Functions */

