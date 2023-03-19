#ifndef STM32F3xx_HAL_I2C_H
#define STM32F3xx_HAL_I2C_H

// #include "stm32f3xx_hal_def.h"

struct I2C_HandleTypeDef {
    uint8_t *pBuffPtr;
};

typedef enum {
    HAL_OK = 0x00U,
    HAL_ERROR = 0x01U,
    HAL_BUSY = 0x02U,
    HAL_TIMEOUT = 0x03
} HAL_StatusTypeDef;

HAL_StatusTypeDef HAL_I2C_Master_Transmit(I2C_HandleTypeDef *hi2c,
                                          uint16_t DevAddress,
                                          uint8_t uint16_t Size,
                                          uint32_t Timeout);

AL_StatusTypeDef HAL_I2C_Master_Receive(I2C_HandleTypeDef *hi2c,
                                        uint16_t DevAddress,
                                        uint8_t uint16_t Size,
                                        uint32_t Timeout);

#endif
