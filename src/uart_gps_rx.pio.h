// -------------------------------------------------- //
// This file is autogenerated by pioasm; do not edit! //
// -------------------------------------------------- //

#if !PICO_NO_HARDWARE
#include "hardware/pio.h"
#endif

// ------- //
// uart_rx //
// ------- //

#define uart_rx_wrap_target 0
#define uart_rx_wrap 8

static const uint16_t uart_rx_program_instructions[] = {
            //     .wrap_target
    0x2020, //  0: wait   0 pin, 0                   
    0xea27, //  1: set    x, 7                   [10]
    0x4001, //  2: in     pins, 1                    
    0x0642, //  3: jmp    x--, 2                 [6] 
    0x00c8, //  4: jmp    pin, 8                     
    0xc014, //  5: irq    nowait 4 rel               
    0x20a0, //  6: wait   1 pin, 0                   
    0x0000, //  7: jmp    0                          
    0x8020, //  8: push   block                      
            //     .wrap
};

#if !PICO_NO_HARDWARE
static const struct pio_program uart_rx_program = {
    .instructions = uart_rx_program_instructions,
    .length = 9,
    .origin = -1,
};

static inline pio_sm_config uart_rx_program_get_default_config(uint offset) {
    pio_sm_config c = pio_get_default_sm_config();
    sm_config_set_wrap(&c, offset + uart_rx_wrap_target, offset + uart_rx_wrap);
    return c;
}

#include "hardware/gpio.h"
#include "hardware/clocks.h"
static inline void uart_rx_program_init(PIO pio, uint sm, uint offset, uint pin_rx, uint baud) {
    //pio_sm_set_pins_with_mask(pio, sm, 1u << pin_tx, 1u << pin_tx); // was in Tx program but is normally not required for Rx
    pio_sm_set_pindirs_with_mask(pio, sm, 0 , 1u << pin_rx); // set pin as input
    //pio_sm_set_consecutive_pindirs(pio, sm, pin, 1, false); // remove by ms
    pio_gpio_init(pio, pin_rx);
    //gpio_set_inover(pin_rx,  GPIO_OVERRIDE_INVERT) ; // added by ms to manage inverted UART from Sport
    //gpio_pull_down(pin_rx); // changed by MS
    gpio_pull_up(pin_rx);
    pio_sm_config c = uart_rx_program_get_default_config(offset);
    sm_config_set_in_pins(&c, pin_rx); // for WAIT, IN
    sm_config_set_jmp_pin(&c, pin_rx); // for JMP
    // Shift to right, autopush disabled
    sm_config_set_in_shift(&c, true, false, 32);
    // Deeper FIFO as we're not doing any TX
    sm_config_set_fifo_join(&c, PIO_FIFO_JOIN_RX);
    // SM transmits 1 bit per 8 execution cycles.
    float div = (float)clock_get_hz(clk_sys) / (8 * baud);
    sm_config_set_clkdiv(&c, div);
    pio_sm_init(pio, sm, offset, &c);
    pio_set_irq0_source_enabled(pio ,  pis_sm1_rx_fifo_not_empty , true ); // added by ms to get an irq int0 from pio
    pio_sm_set_enabled(pio, sm, true);
}
/*static inline void uart_rx_program_stop(PIO pio, uint sm, uint offset, uint pin_rx, uint baud) {
    pio_sm_set_enabled(pio, sm, false); // disable sm.    
}
static inline void uart_rx_program_restart(PIO pio, uint sm, uint offset, uint pin_rx, uint baud) {
    pio_sm_set_pindirs_with_mask(pio, sm, 0 , 1u << pin_rx); // set pin as input
    pio_sm_restart (pio, sm); // to test if we need an enable after this
    pio_sm_set_enabled(pio, sm, true);
}    
static inline char uart_rx_program_getc(PIO pio, uint sm) {
    // 8-bit read from the uppermost byte of the FIFO, as data is left-justified
    io_rw_8 *rxfifo_shift = (io_rw_8*)&pio->rxf[sm] + 3;
    while (pio_sm_is_rx_fifo_empty(pio, sm))
        tight_loop_contents();
    return (char)*rxfifo_shift;
}
*/

#endif
