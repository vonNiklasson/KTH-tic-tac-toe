#include <pic32mx.h>

#define TIMERPERIOD ((80000000 / 64) / 1000)
#if TIMERPERIOD > 0xffff
#error "Timer period is too big."
#endif

void platform_sleep(int milliseconds);
char platform_get_button_state(const char row, const char col);
char platform_get_any_button_state(char *row, char *col);
void platform_set_led(const char row, const char col, char state);
void platform_set_all_led(const unsigned char state);

void _PORTB(int state, int shift);
void _PORTD(int state, int shift);
void _PORTE(int state, int shift);
void _PORTF(int state, int shift);
void _PORTG(int state, int shift);

void platform_sleep(const int milliseconds) {
    //T2CONSET = 0x8000;
    //int i;
    //for (i = 0; i < milliseconds; i++) {
    //    while ((IFS(0) & 0x100) == 0);
    //    IFSCLR(0) = 0x100;
    //}
    //T2CONCLR = 0x8000;
    return;
}

/* Setup inputs & outputs etc */
void platform_init(void) {
    //sets 9 pins to inputs for playfieldbuttons
    TRISDSET = 0b11100001111;
    TRISGSET = 0b1000000000;
    TRISFSET = 0b10;

    //sets 18 pins to outputs for RGB lights
    TRISGCLR = 0b110000000;
    TRISDCLR = 0b100011100000;
    TRISECLR = 0b11111111;
    TRISFCLR = 0b1110000;
    TRISBCLR = 0b10;

    /* Set the prescaling to 64:1
    * bit 4-6 decides the prescaling
    * 0x60 == 01100000 */
    //T2CON = 0x60;
    /* Count from 0 */
    //TMR2 = 0x00;
    /* Count to period set above */
    //PR2 = TIMERPERIOD;
}

/* Gets the state of the button input */
char platform_get_button_state(const char row, const char col) {
    if (row == 0) {
        if (col == 0) {
            return ((PORTD & 0b100000000) >> 8);
        } else if (col == 1) {
            return (PORTD & 0b1);
        } else if (col == 2) {
            return ((PORTF & 0b10) >> 1);
        }
    } else if (row == 1) {
        if (col == 0) {
            return ((PORTD & 0b10) >> 1);
        } else if (col == 1) {
            return ((PORTD &  0b100) >> 2);
        } else if (col == 2) {
            return ((PORTD &  0b1000000000) >> 9);
        }
    } else if (row == 2) {
        if (col == 0) {
            return ((PORTD &  0b10000000000) >> 10);
        } else if (col == 1) {
            return ((PORTD &  0b1000) >> 3);
        } else if (col == 2) {
            return ((PORTG &  0b1000000000) >> 9);
        }
    }
    return 0;
}

/* Returns if a button is pressed, if so returns 1 
 * and set row and col to pressed button */
char platform_get_any_button_state(char *row, char *col) {
    int i;
    int j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (platform_get_button_state(i, j) != 0) {
                *row = i;
                *col = j;
                return 1;
            }
        }
    }
}

/* Lights up a LED in given state (0-2) */
void platform_set_led(const char row, const char col, char state) {
    char light1 = 0;
    char light2 = 0;
    if (state == 1) {
        light1 = 1;
    } else if (state == 2) {
        light2 = 1;
    } else if (state == 3) {
        light1 = 1;
        light2 = 1;
    }

    if (row == 0) {
        if (col == 0) {
            _PORTG(light1, 8);  // GPIO 11
            _PORTE(light2, 7);
        } else if (col == 1) {
            _PORTG(light1, 7);  // GPIO 12
            _PORTD(light2, 5);
        } else if (col == 2) {
            _PORTE(light1, 0);  // GPIO 26
            _PORTD(light2, 11);
        }
    } else if (row == 1) {
        if (col == 0) {
            _PORTE(light1, 1);  // GPIO 27
            _PORTD(light2, 6);
        } else if (col == 1) {
            _PORTE(light1, 2);  // GPIO 28
            _PORTD(light2, 7);
        } else if (col == 2) {
            _PORTE(light1, 3);  // GPIO 29
            _PORTF(light2, 6);
        }
    } else if (row == 2) {
        if (col == 0) {
            _PORTE(light1, 4);  // GPIO 30
            _PORTF(light2, 4);
        } else if (col == 1) {
            _PORTE(light1, 5);  // GPIO 31
            _PORTF(light2, 5);
        } else if (col == 2) {
            _PORTE(light1, 6);  // GPIO 32
            _PORTB(light2, 1);
        }
    }
}

/* Sets all LED to given state (0-2) */
void platform_set_all_led(const unsigned char state) {
    int i;
    int j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            platform_set_led(i, j, state);
        }
    }
}

/* Helper functions to set and clear each port more easily */
void _PORTB(int state, int shift) {
    if (state == 1) {
        PORTBCLR = (1 << shift);
    } else {
        PORTBSET = (1 << shift);
    }
}

void _PORTD(int state, int shift) {
    if (state == 1) {
        PORTDCLR = (1 << shift);
    } else {
        PORTDSET = (1 << shift);
    }
}

void _PORTE(int state, int shift) {
    if (state == 1) {
        PORTECLR = (1 << shift);
    } else {
        PORTESET = (1 << shift);
    }
}

void _PORTF(int state, int shift) {
    if (state == 1) {
        PORTFCLR = (1 << shift);
    } else {
        PORTFSET = (1 << shift);
    }
}

void _PORTG(int state, int shift) {
    if (state == 1) {
        PORTGCLR = (1 << shift);
    } else {
        PORTGSET = (1 << shift);
    }
}
