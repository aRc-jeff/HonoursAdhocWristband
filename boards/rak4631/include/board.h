#pragma once

/**
 * @ingroup     boards_rak4631
 * @{
 *
 * @file
 * @brief       Board specific configuration for the RAK 4631 board
 *
 * @author      Timothy Szabo <a1852299@adelaide.edu.au>
 */

// includes default CPU headers
#include "cpu.h" 

#ifdef __cplusplus
extern "C" {
#endif

// Define DIO2 pin
#define DIO2_PIN                GPIO_PIN(1, 7)

// Define LED and Antenna switch pins and masks 
// mask is (1 << pin_number) regardless of port
#define LED0_PIN                GPIO_PIN(1, 3)
#define LED0_MASK               (1 << 3)
#define LED1_PIN                GPIO_PIN(1, 4)
#define LED1_MASK               (1 << 4)
#define ANT_SW_PIN              GPIO_PIN(1, 5)
#define ANT_SW_MASK             (1 << 5)

// Collect all port 1 devices into a single mask (need separate one for port 0)
#define GPIO_MASK_P1            (LED0_MASK | LED1_MASK | ANT_SW_MASK)

// Define macros to turn LED0 on/off
#define LED0_ON                 (NRF_P1->OUTSET = LED0_MASK)
#define LED0_OFF                (NRF_P1->OUTCLR = LED0_MASK)
#define LED0_TOGGLE             (NRF_P1->OUT   ^= LED0_MASK)

// Define macros to turn LED1 on/off
#define LED1_ON                 (NRF_P1->OUTSET = LED1_MASK)
#define LED1_OFF                (NRF_P1->OUTCLR = LED1_MASK)
#define LED1_TOGGLE             (NRF_P1->OUT   ^= LED1_MASK)

// Define macros to turn Antenna switch on/off
#define ANT_SW_ON               (NRF_P1->OUTSET = ANT_SW_MASK)
#define ANT_SW_OFF              (NRF_P1->OUTCLR = ANT_SW_MASK)
#define ANT_SW_TOGGLE           (NRF_P1->OUT   ^= ANT_SW_MASK)

/**
 * @name        SX126X
 *
 * SX126X configuration, based on sx126x.h
 * @{
 */
#define SX126X_PARAM_SPI                (SPI_DEV(0))
#define SX126X_PARAM_SPI_NSS            GPIO_PIN(1, 10)
#define SX126X_PARAM_RESET              GPIO_PIN(1, 6)
#define SX126X_PARAM_DIO1               GPIO_PIN(1, 15)
#define SX126X_PARAM_BUSY               GPIO_PIN(1, 14)
#define SX126X_PARAM_TYPE               SX126X_TYPE_SX1262
#define SX126X_PARAM_REGULATOR          SX126X_REG_MODE_LDO
#define SX126X_PARAM_DIO2_MODE          SX126X_DIO2_RF_SWITCH
#define SX126X_PARAM_DIO3_MODE          SX126X_DIO3_TCXO
#define SX126X_PARAM_TCXO_VOLTAGE       SX126X_TCXO_CTRL_3_0V
#define SX126X_PARAM_TCXO_TIMEOUT       24
/** @} */

#ifdef __cplusplus
}
#endif
/** @} */

