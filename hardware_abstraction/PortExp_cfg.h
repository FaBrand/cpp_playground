/**
*********************************************************************************
* @file   PortExp_cfg.h
* @brief  Port Expander PCF8575 interface configuration
*
* @details
*
* This file describes the product side data and function interfaces required
* for the port expander driver to operate.
*
*********************************************************************************/
#ifndef PORTEXP_CFG_H_
#define PORTEXP_CFG_H_

/********************************************************************************/
/********************************************************************************/
/* Header files */

#include "stdint.h"
#include "stdbool.h"

/********************************************************************************/
/********************************************************************************/
/* Macros */

/********************************************************************************/
/********************************************************************************/
/* Type definitions */


/** Port Expander device I2C address (full 8 bits) */
typedef struct
{
    uint16_t u16WrAddr;     /**< Write address */
    uint16_t u16RrAddr;     /**< Read  address */

} stPORTEXP_DevI2CAddr_t;


/** Port Expander device descriptor */
typedef struct
{
    uint32_t                    u32Id;          /**< Name/Identifier */
    stPORTEXP_DevI2CAddr_t      stAddr;         /**< IIC address */
    uint16_t                    u16InputMask;   /**< Inputs selection mask*/

} stPORTEXP_DevDesc_t;


/********************************************************************************/
/********************************************************************************/
/* Public Functions (required configuration interfaces) */

/**
 * @brief     Inspect number of instantiated Port Expanders
 * @details
 *
 * This interface is called during the registration process which takes place
 * in the scope of the driver's initialization function. The function provides
 * the number of devices to be registered.
 *
 * @return      Number of port expanders in the system
 *
 */
uint32_t PORTEXP_GetPortExpCount(void );


/**
 * @brief     Inspect Port Expander device descriptor
 * @details
 *
 * This interface is called during the registration process which takes place
 * in the scope of the driver's initialization function. The function provides
 * a reference to the requested device descriptor.
 *
 * @param [in]  u32Idx    - Port Expander ID/Index to provide descriptor for.
 * @return      Reference to port expander descriptor structure
 *
 */
const stPORTEXP_DevDesc_t *PORTEXP_GetPortExpDesc( const uint32_t u32Idx );


/**
 * @brief     Request memory for device operation
 * @details
 *
 * This interface is called during driver initialization to obtain the different
 * memories required for operation.
 *
 * @param [in]  u32Size   - Requested size in bytes count
 * @return      Reference to the provided memory
 *
 */
void *PORTEXP_GetMemory( const uint32_t u32Size );


/**
 * @brief     Transmits Data on the I2C bus
 * @details
 *
 * This interface implements transmissions over the I2C corresponding to the
 * referenced port expander.
 *
 * @param [in]  u32PortExpId - Registered Port Expander ID
 * @param [in]  pu8Data      - Data buffer reference
 * @param [in]  u16Size      - Data buffer size
 * @return      bool         - Operation success
 *
 */
bool PORTEXP_Transmit(
        const uint32_t u32PortExpId,
        uint8_t *const pu8Data,
        const uint16_t u16Size );

/**
 * @brief     Receives Data on the I2C bus
 * @details
 *
 * This interface implements reception over the I2C corresponding to the
 * referenced port expander.
 *
 * @param [in]  u32PortExpId - Registered Port Expander ID
 * @param [in]  pu8Data      - Data buffer reference
 * @param [in]  u16Size      - Data buffer size
 * @return      bool         - Operation success
 *
 */
bool PORTEXP_Receive(
        const uint32_t u32PortExpId,
        uint8_t *const pu8Data,
        const uint16_t u16Size );

#endif /* INC_INTFC_H_ */
