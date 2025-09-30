#pragma once
#include "periph_cpu.h"
#include "cfg_clock_32_1.h"
#include "cfg_rtt_default.h"
#include "cfg_timer_default.h"



// /* HFCLK and LFCLK configuration for nRF52840 */
// #define CLOCK_HFCLK        1
// #define CLOCK_LFCLK        1

/**
 * @name    UART configuration
 * @{
 */
static const uart_conf_t uart_config[] = {
    {
        .dev        = NRF_UARTE0,
        .rx_pin = GPIO_PIN(0, 19),
        .tx_pin = GPIO_PIN(0, 20),
#ifdef MODULE_PERIPH_UART_HW_FC
        .rts_pin    = GPIO_UNDEF,
        .cts_pin    = GPIO_UNDEF,
#endif
        .irqn       = UARTE0_UART0_IRQn,
    },
};

#define UART_0_ISR          (isr_uart0)

#define UART_NUMOF          ARRAY_SIZE(uart_config)
/** @} */

/**
 * @name    SPI configuration
 * @{
 */
static const spi_conf_t spi_config[] = {
    {
        .dev  = NRF_SPIM1,
        .sclk = GPIO_PIN(1, 11),
        .mosi = GPIO_PIN(1, 12),
        .miso = GPIO_PIN(1, 13),
    },
    {
        .dev  = NRF_SPIM2,
        .mosi = GPIO_PIN(0, 17),
    }
};

#define SPI_NUMOF           ARRAY_SIZE(spi_config)
/** @} */

static const i2c_conf_t i2c_config[] = {
    {
        .dev = NRF_TWIM0,
        .scl = GPIO_PIN(0, 14),
        .sda = GPIO_PIN(0, 13),
        .speed = I2C_SPEED_NORMAL
    },
    {
        .dev = NRF_TWIM1,
        .scl = GPIO_PIN(0, 25),
        .sda = GPIO_PIN(0, 24),
        .speed = I2C_SPEED_NORMAL
    },
};
#define I2C_NUMOF           ARRAY_SIZE(i2c_config)




// /* Example: use SPI1 at 8 MHz for the SX1262 */
// static const spi_conf_t spi_conf[SPI_NUMOF] = {
//     /* SPI0 unused */
//     { .dev = NRF_SPIM0, .sck_pin = GPIO_UNDEF, .mosi_pin = GPIO_UNDEF, .miso_pin = GPIO_UNDEF },
//     /* SPI1 used by radio (RAK4631 wiring below) */
//     { .dev = NRF_SPIM1, .sck_pin  = GPIO_PIN(1, 11),  /* SCK  */
//                         .mosi_pin = GPIO_PIN(1, 12),  /* MOSI */
//                         .miso_pin = GPIO_PIN(1, 13),  /* MISO */
//       .freq = SPI_FREQ_8MHZ, .mode = SPI_MODE_0, .orc = 0xFF },
//     /* SPI2 unused */
//     { .dev = NRF_SPIM2, .sck_pin = GPIO_UNDEF, .mosi_pin = GPIO_UNDEF, .miso_pin = GPIO_UNDEF },
// };
// #define SPI_NUMOF ARRAY_SIZE(spi_config)
