
#include "uart.h"
#include <avr/io.h>
#include <avr/interrupt.h>

/* --------- RX ring (optional) disabled here to keep it minimal --------- */

static uint16_t calc_baud_reg(uint32_t baud) {
    /* Normal mode (SAMPR=0): BAUD = (64*F_CPU)/(16*baud) = (F_CPU*4)/baud */
    uint32_t v = ((uint32_t)F_CPU * 4u) / baud;
    if (v > 0xFFFFu) v = 0xFFFFu;
    return (uint16_t)v;
}

void uart_init(uint32_t baud, uart_route_t route)
{
    /* Select pin route (PORTMUX.CTRLB bit USART0 on tiny/mega 0/1 series) */
    if (route == UART_ROUTE_ALT) {
        /* ALT = PA1 TXD0, PA2 RXD0 */
        PORTMUX.CTRLB |= PORTMUX_USART0_bm;            // select alternate
        PORTA.DIRSET = PIN1_bm;                         // PA1 (TX) out
        PORTA.OUTSET = PIN1_bm;                         // idle high
        // Optional RX pull-up: PORTA.PIN2CTRL = PORT_PULLUPEN_bm;
    } else {
        /* DEFAULT = PB2 TXD0, PB3 RXD0 */
        PORTMUX.CTRLB &= ~PORTMUX_USART0_bm;           // select default
        PORTB.DIRSET = PIN2_bm;                         // PB2 (TX) out
        PORTB.OUTSET = PIN2_bm;                         // idle high
        // Optional RX pull-up: PORTB.PIN3CTRL = PORT_PULLUPEN_bm;
    }

    /* USART0: 8N1, async, normal sampling */
    USART0.CTRLA = 0;                         // no interrupts (simple TX-only)
    USART0.CTRLB = 0;
    USART0.CTRLC = USART_CMODE_ASYNCHRONOUS_gc
                 | USART_PMODE_DISABLED_gc
                 | USART_SBMODE_1BIT_gc
                 | USART_CHSIZE_8BIT_gc;

    USART0.BAUD  = calc_baud_reg(baud);
    USART0.CTRLB = USART_TXEN_bm;             // TX enable (RX optional)
}

void uart_write(uint8_t byte) {
    while (!(USART0.STATUS & USART_DREIF_bm)) { }
    USART0.TXDATAL = byte;
}

void uart_write_bytes(const uint8_t *data, size_t len) {
    for (size_t i=0;i<len;i++) uart_write(data[i]);
}

void uart_print(const char *s) {
    while (*s) {
        if (*s == '\n') uart_write('\r');
        uart_write((uint8_t)*s++);
    }
}


