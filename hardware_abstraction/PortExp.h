/**
*********************************************************************************
* @file   PortExp.h
* @brief  Port Expander PCF8575 interface
*
* @details
*
* This file contains the function and data interfaces to drive PCF8575, 16bits
* port expanders.
*
*********************************************************************************/
#ifndef PORTEXP_H_
#define PORTEXP_H_

/********************************************************************************/
/********************************************************************************/
/* Header files */
#include <stdint.h>
#include <stdio.h>

/********************************************************************************/
/********************************************************************************/
/* Macros */

/********************************************************************************/
/********************************************************************************/
/* Type definitions */

/********************************************************************************/
/********************************************************************************/
/* Type definitions */
/********************************************************************************/

/** Port expander, individual pin states */
typedef enum {
    PORTEXP_PIN_STATE__CLR, /**< GND/Clear */
    PORTEXP_PIN_STATE__SET, /**< VCC/Set   */

} ePORTEXP_PinState_t;

/** Driver Errors data */
typedef struct {
    uint8_t u8ErrBitSet; /**< Latching error bits */
    uint8_t u8ErrCnt;    /**< Occurrence count */

} stPORTEXP_ErrData_t;

/********************************************************************************/
/* Public Functions */

/**
 * @brief     PORTEXP_Init
 * @details

 * Called during initialization, prior the driver becomes operational.
 *
 * @return      None
 *
 */
void PORTEXP_Init(void);

/**
 * @brief     PORTEXP_UpdatePinBuffer
 * @details

 * Updates the value of a port's output bit/pin in the driver's internal buffer.
 * Attempts to write pins qualified as inputs are ignored.
 *
 * @param [in]  u32PortExpId - Registered Port Expander ID
 * @param [in]  u8PinNum     - Target Pin/Bit number
 * @param [in]  ePinState    - New Pin/Bit state
 * @return      None
 *
 */
void PORTEXP_UpdatePinBuffer(const uint32_t u32PortExpId,
                             const uint8_t u8PinNum,
                             const ePORTEXP_PinState_t ePinState);

/**
 * @brief     PORTEXP_UpdatePortBuffer
 * @details

 * Updates the value of a port in the driver's internal buffer.
 * Port bits qualified as inputs are ignored.
 *
 * @param [in]  u32PortExpId - Registered Port Expander ID
 * @param [in]  u16PortValue - New port output value
 * @return      None
 *
 */
void PORTEXP_UpdatePortBuffer(const uint32_t u32PortExpId,
                              const uint16_t u16PortValue);

/**
 * @brief     PORTEXP_SyncBuffer
 * @details

 * Synchronizes the expander's port and the corresponding driver's internal
 * buffer. Inputs read from the expander port are copied into the buffer and
 * expander port outputs are updated with values from the buffer.
 *
 * @param [in]  u32PortExpId - Registered Port Expander ID
 * @return      uint16_t     - Buffer value after synchronization
 *
 */
uint16_t PORTEXP_SyncBuffer(const uint32_t u32PortExpId);

/**
 * @brief     PORTEXP_InspectPortBuffer
 * @details

 * Inspects the current the value held in the driver's internal buffer
 * corresponding to a port.
 *
 * @param [in]  u32PortExpId - Registered Port Expander ID
 * @return      uint16_t     - Buffer value
 *
 */
uint16_t PORTEXP_InspectPortBuffer(const uint32_t u32PortExpId);

/**
 * @brief     PORTEXP_InspectPinBuffer
 * @details

 * Inspects a Pin/Bit state held in the driver's internal buffer corresponding
 * to a port.
 *
 * @param [in]  u32PortExpId - Registered Port Expander ID
 * @return      uint16_t     - Buffer value
 *
 */
ePORTEXP_PinState_t PORTEXP_InspectPinBuffer(const uint32_t u32PortExpId,
                                             const uint8_t u8PinNum);

/**
 * @brief     PORTEXP_ClearErrors
 * @details

 * Clears latched error flags and occurrence counters. This operation is only
 * allowed after driver initialization.
 *
 * @return      none
 *
 */
void PORTEXP_ClearErrors(void);

/**
 * @brief     PORTEXP_InspectErrorsData
 * @details

 * Inspect error states and occurrence counters.
 *
 * @return      stPORTEXP_ErrData_t - Errors data
 *
 */
stPORTEXP_ErrData_t PORTEXP_InspectErrorsData(void);

#endif /* INC_INTFC_H_ */
