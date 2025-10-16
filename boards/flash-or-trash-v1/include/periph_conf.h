#pragma once

// Include default cpu peripheral headers
#include "periph_cpu.h"

// Include cpu clock, counter and timer configurations 
// (clock file depends on hardware config)
// CLOCK_HFCLK  (32U): 32 mHz crystal main clock
// CLOCK_LFCLK CLOCK_LFCLKSRC_SRC_Xtal: 32.768 kHz crystal
#include "cfg_clock_32_1.h"
#include "cfg_rtt_default.h"
#include "cfg_timer_default.h"


//UART configuration is required for RIOT to build, we arent actually using it
/**
 * @name    UART configuration
 * @{
 */
static const uart_conf_t uart_config[] = {
    {
        // controller selection
        .dev        = NRF_UARTE0,
        // pin configuration
        .rx_pin = GPIO_PIN(0, 19),
        .tx_pin = GPIO_PIN(0, 20),
        // if the modules are expecting these pins... they arent connected
#ifdef MODULE_PERIPH_UART_HW_FC
        .rts_pin    = GPIO_UNDEF,
        .cts_pin    = GPIO_UNDEF,
#endif
        // interrupt config
        .irqn       = UARTE0_UART0_IRQn,
    },
};

// configure UART interrupt service routine
#define UART_0_ISR          (isr_uart0)

// set the number of configured uart devices
#define UART_NUMOF          ARRAY_SIZE(uart_config)
/** @} */

/**
 * @name    SPI configuration
 * @{
 */
static const spi_conf_t spi_config[] = {
    {
        // select SPI controller for SX1262 radio 
        // (0, 1 and 2 are low speed, 3 is high speed)
        .dev  = NRF_SPIM1,

        // configure spi pins
        .sclk = GPIO_PIN(1, 11),
        .mosi = GPIO_PIN(1, 12),
        .miso = GPIO_PIN(1, 13),
    },
    {
        // select SPI controller for ws2812 LED
        .dev  = NRF_SPIM2,
        .mosi = GPIO_PIN(0, 17),
    }
};

// set the number of configured SPI devices
#define SPI_NUMOF           ARRAY_SIZE(spi_config)
/** @} */

// I2C configured on the rak4631 but not actually used
/**
 * @name    I2C configuration
 * @{
 */
static const i2c_conf_t i2c_config[] = {
    {
        // select i2c controller
        .dev = NRF_TWIM0,
        // set i2c pins
        .scl = GPIO_PIN(0, 14),
        .sda = GPIO_PIN(0, 13),
        // set i2c speed
        .speed = I2C_SPEED_NORMAL
    },
    {
        // select i2c controller
        .dev = NRF_TWIM1,
        // set i2c pins
        .scl = GPIO_PIN(0, 25),
        .sda = GPIO_PIN(0, 24),
        // set i2c speed
        .speed = I2C_SPEED_NORMAL
    },
};
// set the number of configured I2C devices
#define I2C_NUMOF           ARRAY_SIZE(i2c_config)
/** @} */
