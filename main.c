#define F_CPU 3333333UL
#include <avr/io.h>
#include "uart.h"
#include <util/delay.h>

#define LED_PA4 PIN4_bm

int main(void)
{
    /* LED on PA4 just to mirror your Arduino test */
    PORTA.DIRSET = LED_PA4;

    /* If your adapter is wired to PB2 (recommended default), use DEFAULT */
    uart_init(9600, UART_ROUTE_DEFAULT);
    // If you really wired to PA1, use: uart_init(9600, UART_ROUTE_ALT);

    for (;;) {
        PORTA.OUTSET = LED_PA4;
        uart_print("test uart ABCDEFGHIJKLMNOPQRSTUVWXYZ\r\n");
       _delay_ms(1000); //for (volatile uint32_t i=0; i< F_CPU/5; i++) { } 
        PORTA.OUTCLR = LED_PA4;
       // for (volatile uint32_t i=0; i< F_CPU/5; i++) { }
          _delay_ms(1000); 
    }
}



