// -------------------------------------------------- //
// This file is autogenerated by pioasm; do not edit! //
// -------------------------------------------------- //

#pragma once

#if !PICO_NO_HARDWARE
#include "hardware/pio.h"
#endif

// ------------ //
// uart_sbus_tx //
// ------------ //

#define uart_sbus_tx_wrap_target 0
#define uart_sbus_tx_wrap 3

static const uint16_t uart_sbus_tx_program_instructions[] = {
            //     .wrap_target
    0x9fa0, //  0: pull   block           side 1 [7] 
    0xf729, //  1: set    x, 9            side 0 [7] 
    0x6001, //  2: out    pins, 1                    
    0x0642, //  3: jmp    x--, 2                 [6] 
            //     .wrap
};

#if !PICO_NO_HARDWARE
static const struct pio_program uart_sbus_tx_program = {
    .instructions = uart_sbus_tx_program_instructions,
    .length = 4,
    .origin = -1,
};

static inline pio_sm_config uart_sbus_tx_program_get_default_config(uint offset) {
    pio_sm_config c = pio_get_default_sm_config();
    sm_config_set_wrap(&c, offset + uart_sbus_tx_wrap_target, offset + uart_sbus_tx_wrap);
    sm_config_set_sideset(&c, 2, true, false);
    return c;
}

#include "hardware/clocks.h"
static inline void uart_sbus_tx_program_init(PIO pio, uint sm, uint offset, uint pin_tx, uint baud) {
    // Tell PIO to initially drive output-high on the selected pin, then map PIO
    // onto that pin with the IO muxes.
    pio_sm_set_pins_with_mask(pio, sm, 1u << pin_tx, 1u << pin_tx);
    pio_sm_set_pindirs_with_mask(pio, sm, 1u << pin_tx, 1u << pin_tx);
    pio_gpio_init(pio, pin_tx);
    gpio_set_outover(pin_tx,  GPIO_OVERRIDE_INVERT) ; // added by ms to manage inverted UART from Sbus
    gpio_pull_down(pin_tx); // changed by MS
    pio_sm_config c = uart_sbus_tx_program_get_default_config(offset);
    // OUT shifts to right, no autopull
    sm_config_set_out_shift(&c, true, false, 32);
    // We are mapping both OUT and side-set to the same pin, because sometimes
    // we need to assert user data onto the pin (with OUT) and sometimes
    // assert constant values (start/stop bit)
    sm_config_set_out_pins(&c, pin_tx, 1);
    sm_config_set_sideset_pins(&c, pin_tx);
    // We only need TX, so get an 8-deep FIFO!
    sm_config_set_fifo_join(&c, PIO_FIFO_JOIN_TX);
    // SM transmits 1 bit per 8 execution cycles.
    float div = (float)clock_get_hz(clk_sys) / (8 * baud);
    sm_config_set_clkdiv(&c, div);
    pio_sm_init(pio, sm, offset, &c);
    pio_sm_set_enabled(pio, sm, true);
}
/*
static inline void uart_tx_program_putc(PIO pio, uint sm, char c) {
    pio_sm_put_blocking(pio, sm, (uint32_t)c);
}
static inline void uart_tx_program_puts(PIO pio, uint sm, const char *s) {
    while (*s)
        uart_tx_program_putc(pio, sm, *s++);
}
*/

#endif

