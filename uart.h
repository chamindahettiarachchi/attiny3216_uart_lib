#ifndef UART_H
#define UART_H
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Set your real clock in all files that compile this header */
#define F_CPU 3333333UL

typedef enum {
    UART_ROUTE_DEFAULT = 0,   // PB2=TXD0, PB3=RXD0  <-- ATtiny3216 default
    UART_ROUTE_ALT            // PA1=TXD0, PA2=RXD0  <-- alternate via PORTMUX
} uart_route_t;

void uart_init(uint32_t baud, uart_route_t route);
void uart_write(uint8_t byte);
void uart_write_bytes(const uint8_t *data, size_t len);
void uart_print(const char *s);

#endif
