/*
 * Name: Johan Niklasson
 * Project: Half duplex protocol
 *
 * Source: src/protocol_platform_pic32mx.c
 */

#include "protocol_platform.h"
#include "protocol.h"
#include <pic32mx.h>

/******************** Delay functions below ********************/

#define TMR2PERIOD ((80000000 / 64) / BITRATE_BASE)
#if TMR2PERIOD > 0xffff
#error "Timer period is too big."
#endif

/* Define a delay setup here (will be called when protocol inits, may be more than once) */
void platform_delay_setup() {
  /* Set the prescaling to 64:1
   * bit 4-6 decides the prescaling
   * 0x60 == 01100000 */
    T2CON = 0x60;
    /* Count from 0 */
    TMR2 = 0x00;
    /* Count to period set above */
    PR2 = TMR2PERIOD;
}

/* Define a delay pre transfer here (will be called before transfer starts)
 * recieve: false=sending data, true=recieving data */
void platform_delay_pre_transfer(const bool recieve) {
    /* Starts Timer 2 */
    T2CONSET = 0x8000;
    return;
}

/* Define a delay post transfer here (will be called after transfer is done)
 * recieve: false=sending data, true=recieving data */
void platform_delay_post_transfer(const bool recieve) {
    /* Stops Timer 2 */
    T2CONCLR = 0x8000;
    return;
}

/* Define a delay here */
void platform_delay(const int milliseconds) {
    int i;
    for (i = 0; i < milliseconds; i++) {
        while ((IFS(0) & 0x100) == 0);
        IFSCLR(0) = 0x100;
    }
    return;
}

/* Define a delay here that also check for state change on the gpio input port */
void platform_delay_or_gpio_change(const int milliseconds) {
    int initial_state = platform_gpio_read();
    int i;
    for (i = 0; i < milliseconds; i++) {
        platform_delay(1);
        if (initial_state != platform_gpio_read()) {
            return;
        }
    }
    return;
}


/******************** GPIO functions below ********************/

/* Setup gpio ports here */
void platform_gpio_setup(void) {
    // Recieve on GPIO 13
    TRISGSET = 0b1000000;

    // Transmit on port A
    TRISBCLR = 0b1;
    PORTBCLR = 0b1;
    return;
}

/* Define a gpio pre transfer here (will be called before transfer starts)
 * recieve: false=sending data, true=recieving data */
void platform_gpio_pre_transfer(const bool recieve) {
    if (recieve) {
        //TRISBSET = 0x1;
        //TRISGSET = 0b1000000;
    } else {
        //TRISGCLR = 0b1000000;
        //PORTGCLR = 0b1000000;
        //TRISBCLR = 0x1;
        //PORTBCLR = 0x1;
    }
    return;
}

/* Define a gpio post transfer here (will be called after transfer is done)
 * recieve: false=sending data, true=recieving data */
void platform_gpio_post_transfer(const bool recieve) {
    if (recieve) {

    } else {

    }
    return;
}

/* Doesn't need modification */
void platform_gpio_set(const int state) {
    if (state) {
        platform_gpio_set_high();
    } else {
        platform_gpio_set_low();
    }
}

/* Setup procedure on gpio high here */
void platform_gpio_set_high(void) {
    PORTBSET = 0x1;
    PORTGSET = 0b1000000;
    return;
}

/* Setup procedure on gpio low here */
void platform_gpio_set_low(void) {
    PORTBCLR = 0x1;
    return;
}


/* Setup procedure on reading the gpio here */
int platform_gpio_read(void) {
    return ((PORTG & 0b1000000) >> 6);
}


/******************** DEBUG functions below ********************/

/* Setup eventual function to debug */
void platform_debug(const char *str, const int val) {
    return;
}
