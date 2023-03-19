/**
*********************************************************************************
* @file   PortExp.h
* @brief  Port Expander PCF8575 interface
*
* @details
*
* This file contains the functionality required to interface with PCF8575,
* 16 bits port expanders.
*
*********************************************************************************/

/********************************************************************************/
/********************************************************************************/
/* Header files */
#include <stdio.h>

#include "PortExp_cfg.h"
#include "PortExp.h"

/********************************************************************************/
/********************************************************************************/
/* Macros */

/** Pin count on each registered port, only 16 bits ports are supported */
#define PORT_EXP_PIN_COUNT      (16)

#define PORT_EXP_ERR_INTFC      (1<<0)
#define PORT_EXP_ERR_COMMS      (1<<1)

/********************************************************************************/
/********************************************************************************/
/* Type definitions */

/** Driver operational states */
typedef enum
{
    PORTEXP_DRV_STATE__STARTUP,         /**< Startup, initialization pending */
    PORTEXP_DRV_STATE__OPERATIONAL,     /**< Operational */
    PORTEXP_DRV_STATE__ERROR,           /**< Non operational, in error */

} ePortExpDrvState_t;

/** Driver error types */
typedef enum
{
    PORTEXP_DRV_ERR_INTFC = 0,          /**< Interface usage error */
    PORTEXP_DRV_ERR_CONFG = 1,          /**< Driver configuration error */
    PORTEXP_DRV_ERR_COMMS = 2,          /**< I2C communications error */

} ePortExpErr_t;

/** Port Expander device data definition */
typedef struct
{
    const stPORTEXP_DevDesc_t *pstDesc;     /**< Reference to device descriptor */
    uint16_t u16Data;                       /**< Buffered port data */

} stPortExpDevData_t;

/** Port Expander driver data definition */
typedef struct
{
    uint32_t                u32PortExpCnt;      /**< Number of port expanders registered */
    stPortExpDevData_t      *pstPortExpData;    /**< Port expanders' data */
    stPORTEXP_ErrData_t     stErrData;          /**< Errors data */

} stPortExpDrvData_t;

/********************************************************************************/
/********************************************************************************/
/* Working variables */

/** Driver state  */
static ePortExpDrvState_t eDrvState = PORTEXP_DRV_STATE__STARTUP;

/** Driver data */
static stPortExpDrvData_t stDrvData;

/********************************************************************************/
/********************************************************************************/
/* Local Function Prototypes */

/**
 * @brief   Updates Port Expander I/O
 * @details
 *
 * This function issues read/write transactions to a Port Expander updating
 * output pins and reading input pins.
 *
 * @param [in]  pstData - Reference to device descriptor and buffered port data
 * @return      None
 */
static void UpdatePortExp( stPortExpDevData_t *const pstData );

/**
 * @brief   Clear errors
 * @details
 *
 * This function clears recorded errors flags and counters.
 *
 * @return      None
 */
static void ClrErrorData(void);

/**
 * @brief   Records error occurrence
 * @details
 *
 * This function asserts an error flag and increments the overall errors
 * occurrence counter up to saturation point.
 *
 * @param [in]  eErr - Error type
 * @return      None
 */
static void SetError( const ePortExpErr_t eErr );


/********************************************************************************/
/********************************************************************************/
/* Public Functions */

/* */
void PORTEXP_Init(void)
{
    const stPORTEXP_DevDesc_t *pstDesc;
    stPortExpDevData_t *pstData;

    if( PORTEXP_DRV_STATE__STARTUP == eDrvState )
    {
        /* Inspect total number of port expander devices to register */
        stDrvData.u32PortExpCnt = PORTEXP_GetPortExpCount();

        /* Allocate working data */
        stDrvData.pstPortExpData = (stPortExpDevData_t *)PORTEXP_GetMemory(
                sizeof(stPortExpDevData_t) * stDrvData.u32PortExpCnt );

        /* Proceed with registratin if possible  */
        if( NULL != stDrvData.pstPortExpData )
        {
            for( uint32_t u32PortIdx = 0; u32PortIdx < stDrvData.u32PortExpCnt; u32PortIdx++ )
            {
                /* Reduce scope */
                pstDesc = PORTEXP_GetPortExpDesc( u32PortIdx );

                /* Verify descriptor */
                if(  ( NULL != pstDesc )
                  && ( pstDesc->u32Id == u32PortIdx ) )
                {
                    /* Reduce scope */
                    pstData = &stDrvData.pstPortExpData[u32PortIdx];

                    /* Link working data and port descriptor */
                    pstData->pstDesc = pstDesc;

                    /* Default weak drive */
                    pstData->u16Data = 0xFFFF;

                    /* Initial/default update */
                    UpdatePortExp( pstData);
                }
                else
                {
                    /* Integration error */
                    eDrvState = PORTEXP_DRV_STATE__ERROR;

                }
            }
        }
        else
        {
            /* Integration error */
            eDrvState = PORTEXP_DRV_STATE__ERROR;
        }

        if( eDrvState != PORTEXP_DRV_STATE__ERROR )
        {
            /* Registration completed successful */
            eDrvState = PORTEXP_DRV_STATE__OPERATIONAL;
        }
        else
        {
            /* Configuration / resources error */
            SetError( PORTEXP_DRV_ERR_CONFG );
        }
    }
    else
    {
        /* Initialization already carried out */
        SetError( PORTEXP_DRV_ERR_INTFC );
    }
}

/* */
void PORTEXP_UpdatePinBuffer(
        const uint32_t u32PortExpId,
        const uint8_t u8PinNum,
        const ePORTEXP_PinState_t ePinState )
{
    if(  ( PORTEXP_DRV_STATE__OPERATIONAL == eDrvState )
      && ( u32PortExpId < stDrvData.u32PortExpCnt )
      && ( u8PinNum < PORT_EXP_PIN_COUNT ) )
    {
        /* Reduce scope */
        stPortExpDevData_t *const pstData = &stDrvData.pstPortExpData[u32PortExpId];

        /* Prepare output pin mask, ignore if marked as inputs */
        const uint16_t u16PinMask = (1U << u8PinNum) & (~pstData->pstDesc->u16InputMask);

        /* Update port data buffer */
        if( PORTEXP_PIN_STATE__CLR == ePinState )
        {
            pstData->u16Data &= ~u16PinMask;
        }
        else
        {
            pstData->u16Data |= u16PinMask;
        }
    }
    else
    {
        /* Request out of range */
        SetError( PORTEXP_DRV_ERR_INTFC );
    }
}

void PORTEXP_UpdatePortBuffer(
        const uint32_t u32PortExpId,
        const uint16_t u16PortValue )
{
    if(  ( PORTEXP_DRV_STATE__OPERATIONAL == eDrvState )
      && ( u32PortExpId < stDrvData.u32PortExpCnt ) )
    {
        /* Reduce scope */
        stPortExpDevData_t *const pstData = &stDrvData.pstPortExpData[u32PortExpId];

        /* Update output bits in port buffer*/
        pstData->u16Data = u16PortValue | pstData->pstDesc->u16InputMask;
    }
    else
    {
        /* Request out of range */
        SetError( PORTEXP_DRV_ERR_INTFC );
    }
}

uint16_t PORTEXP_SyncBuffer( const uint32_t u32PortExpId )
{
    uint16_t u16PortBuffer = 0xFFFF;

    if(  ( PORTEXP_DRV_STATE__OPERATIONAL == eDrvState )
      && ( u32PortExpId < stDrvData.u32PortExpCnt ) )
    {
        /* Reduce scope */
        stPortExpDevData_t *const pstData = &stDrvData.pstPortExpData[u32PortExpId];

        /* Push port buffer onto port expander update */
        UpdatePortExp(pstData);
    }
    else
    {
        /* Request out of range */
        SetError( PORTEXP_DRV_ERR_INTFC );
    }

    return u16PortBuffer;
}

uint16_t PORTEXP_InspectPortBuffer( const uint32_t u32PortExpId )
{
    uint16_t u16PortBuffer = 0xFFFF;

    if( u32PortExpId < stDrvData.u32PortExpCnt )
    {
        /* Reduce scope */
        stPortExpDevData_t *const pstData = &stDrvData.pstPortExpData[u32PortExpId];

        /* Inspect data */
        u16PortBuffer = pstData->u16Data;
    }
    else
    {
        /* Request out of range */
        SetError( PORTEXP_DRV_ERR_INTFC );
    }

    return u16PortBuffer;
}

ePORTEXP_PinState_t PORTEXP_InspectPinBuffer(
        const uint32_t u32PortExpId,
        const uint8_t u8PinNum )
{
    ePORTEXP_PinState_t ePinState = PORTEXP_PIN_STATE__CLR;

    if(  ( PORTEXP_DRV_STATE__OPERATIONAL == eDrvState )
            && ( u8PinNum < PORT_EXP_PIN_COUNT ) )
    {
        /* Reduce scope */
        stPortExpDevData_t *const pstData = &stDrvData.pstPortExpData[u32PortExpId];

        /* Prepare pin mask */
        const uint16_t u16PinMask = (1U << u8PinNum);

        if( 0 != (u16PinMask & pstData->u16Data) )
        {
            ePinState = PORTEXP_PIN_STATE__SET;
        }
    }
    else
    {
        /* Request out of range */
        SetError( PORTEXP_DRV_ERR_INTFC );
    }

    return ePinState;
}

void PORTEXP_ClearErrors(void)
{
    /* Operation only allowed after devices registration */
    if( PORTEXP_DRV_STATE__OPERATIONAL ==  eDrvState )
    {
        ClrErrorData();
    }
    else
    {
        SetError(PORTEXP_DRV_ERR_INTFC);
    }
}


stPORTEXP_ErrData_t PORTEXP_InspectErrorsData(void)
{
    return stDrvData.stErrData;
}

/********************************************************************************/
/********************************************************************************/
/* Local Functions */

static void UpdatePortExp( stPortExpDevData_t *const pstData )
{
    bool bTxState;
    bool bRxState;
    uint16_t u16DataOut;
    uint16_t u16DataIn;

    /* Mask out inputs from data buffer */
    u16DataOut = pstData->u16Data | pstData->pstDesc->u16InputMask;

    /* Drive outputs & read inputs  */
    bTxState = PORTEXP_Transmit( pstData->pstDesc->u32Id, (uint8_t*)&u16DataOut, 2 );
    bRxState = PORTEXP_Receive ( pstData->pstDesc->u32Id, (uint8_t*)&u16DataIn,  2 );

    if(  ( true == bTxState )
      && ( true == bRxState ) )
    {
        /* Update input values in data buffer */
        pstData->u16Data &= ~(pstData->pstDesc->u16InputMask);
        pstData->u16Data |= u16DataIn & pstData->pstDesc->u16InputMask;
    }
    else
    {
        /* I2C communications appear to have failed */
        SetError( PORTEXP_DRV_ERR_COMMS );
    }
}

static void ClrErrorData(void)
{
    /* Clear error bits and occurrence counter */
    stDrvData.stErrData.u8ErrBitSet = 0;
    stDrvData.stErrData.u8ErrCnt = 0;
}

static void SetError( const ePortExpErr_t eErr )
{
    /* Assert corresponding error flag */
    stDrvData.stErrData.u8ErrBitSet |= (1U << (uint8_t)eErr );

    /* Increment errors occurence counter up to saturation point */
    if( stDrvData.stErrData.u8ErrCnt < 255 )
    {
        stDrvData.stErrData.u8ErrCnt++;
    }
}
